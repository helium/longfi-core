#include "blake2.h"
#include "lfc/fingerprint.h"
#include "lfc/lfc.h"
#include <assert.h>

#define B2_RES(eXpr_)                                                          \
    do {                                                                       \
        if (eXpr_) {                                                           \
            return lfc_res_err_exception;                                      \
        }                                                                      \
    } while (0)

#define LFC_RES(eXpr_)                                                         \
    do {                                                                       \
        enum lfc_res LrEs = (eXpr_);                                           \
        if (LrEs != lfc_res_ok) {                                              \
            return LrEs;                                                       \
        }                                                                      \
    } while (0)


enum lfc_res
lfc_payload_hash(uint8_t const * pay, size_t pay_len, uint8_t * out, size_t out_len) {
    B2_RES(blake2s(out, out_len, pay, pay_len, NULL, 0));
    return lfc_res_ok;
}


enum lfc_res
lfc_fingerprint_monolithic(uint8_t const * key,
                           size_t          key_len,
                           uint16_t        hdr,
                           uint32_t        oui,
                           uint32_t        did,
                           uint32_t        seq,
                           uint8_t const * pay,
                           size_t          pay_len,
                           uint32_t *      out) {
    uint32_t const out_enter = *out;
    (void)out_enter;
    blake2s_state b2s;
    if (key && key_len) {
        B2_RES(blake2s_init_key(&b2s, sizeof(*out), key, key_len));
    } else {
        B2_RES(blake2s_init(&b2s, sizeof(*out)));
    }
    B2_RES(blake2s_update(&b2s, &hdr, sizeof(hdr)));
    B2_RES(blake2s_update(&b2s, &oui, sizeof(oui)));
    B2_RES(blake2s_update(&b2s, &did, sizeof(did)));
    B2_RES(blake2s_update(&b2s, &seq, sizeof(seq)));
    uint8_t pay_hash[BLAKE2S_OUTBYTES];
    LFC_RES(lfc_payload_hash(pay, pay_len, pay_hash, sizeof(pay_hash)));
    B2_RES(blake2s_update(&b2s, pay_hash, sizeof(pay_hash)));
    B2_RES(blake2s_final(&b2s, out, sizeof(*out)));
    assert(out_enter != *out);
    assert(lfc_fingerprint_monolithic_verify(
               *out, key, key_len, pay_hash, sizeof(pay_hash), hdr, oui, did, seq)
           == lfc_res_ok);
    return lfc_res_ok;
}


enum lfc_res
lfc_fingerprint_monolithic_verify(uint32_t        fp_decl,
                                  uint8_t const * key,
                                  size_t          key_len,
                                  uint8_t const * pay_hash,
                                  size_t          pay_hash_len,
                                  uint16_t        hdr,
                                  uint32_t        oui,
                                  uint32_t        did,
                                  uint32_t        seq) {
    uint32_t      fp_local = 0;
    blake2s_state b2s;
    if (key && key_len) {
        B2_RES(blake2s_init_key(&b2s, sizeof(fp_local), key, key_len));
    } else {
        B2_RES(blake2s_init(&b2s, sizeof(fp_local)));
    }
    B2_RES(blake2s_update(&b2s, &hdr, sizeof(hdr)));
    B2_RES(blake2s_update(&b2s, &oui, sizeof(oui)));
    B2_RES(blake2s_update(&b2s, &did, sizeof(did)));
    B2_RES(blake2s_update(&b2s, &seq, sizeof(seq)));
    B2_RES(blake2s_update(&b2s, pay_hash, pay_hash_len));
    B2_RES(blake2s_final(&b2s, &fp_local, sizeof(fp_local)));
    return fp_local == fp_decl ? lfc_res_ok : lfc_res_err_fingerprint;
}
