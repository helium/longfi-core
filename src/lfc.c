#include "lfc/lfc.h"
#include "blake2.h"
#include "golay/golay.h"
#include "lfc/datagram.h"
#include "lfc/priv/lfc_dg_ser.h"
#include <assert.h>
#include <string.h>

#define B2_RES(eXpr_)                                                          \
    do {                                                                       \
        int res = (eXpr_);                                                     \
        if (res) {                                                             \
            lfc_res_err_exception;                                             \
        }                                                                      \
    } while (0)

#define SER_RES(eXpr_)                                                         \
    do {                                                                       \
        enum lfc_dg_ser_res res = (eXpr_);                                     \
        if (res != lfc_dg_ser_res_ok) {                                        \
            lfc_res_err_exception;                                             \
        }                                                                      \
    } while (0)

/* Returns and increments sequence number. */
uint32_t
lfc_seq(struct lfc * lfc);


void
lfc_init(struct lfc * lfc, struct lfc_user_cfg cfg) {
    *lfc = (struct lfc){
        .seq = 0,
        .cfg = cfg,
    };
}


/* TODO: as a quick PoC we're the encoded datagram type as monolithic,
 *       and computing fingerprint in a way that assumes all the CPU
 *       is little endian. Clean this up, and possibly move
 *       fingerprinting to the serialization routines. */
enum lfc_res
lfc_transmit(struct lfc *    lfc,
             uint8_t const * pay,
             size_t          pay_len,
             uint8_t *       out,
             size_t *        out_len) {
    assert(pay_len <= LFC_DG_CONSTANTS_MAX_PAY_LEN);

    struct lfc_dg_monolithic dg =
        (struct lfc_dg_monolithic){.flags =
                                       (struct lfc_dg_monolithic_flags){
                                           .downlink   = false,
                                           .should_ack = false,
                                           .cts_rts    = false,
                                           .priority   = false,
                                           .ldpc       = false,
                                       },
                                   .oui     = lfc->cfg.oui,
                                   .did     = lfc->cfg.did,
                                   .seq     = lfc_seq(lfc),
                                   .pay_len = pay_len};
    memcpy(dg.pay, pay, pay_len);
    /* Assuming no set flags for this PoC. */
    uint8_t       flag_bits = 0;
    uint16_t      hdr       = (lfc_dg_type_monolithic << 6) | flag_bits;
    blake2s_state b2s;

    /* Compute fingerprint. */
    B2_RES(blake2s_init(&b2s, sizeof(dg.did)));
    B2_RES(blake2s_update(&b2s, &hdr, sizeof(hdr)));
    B2_RES(blake2s_update(&b2s, &dg.oui, sizeof(dg.oui)));
    B2_RES(blake2s_update(&b2s, &dg.did, sizeof(dg.did)));
    B2_RES(blake2s_update(&b2s, &dg.seq, sizeof(dg.seq)));
    uint8_t hash_pay[BLAKE2S_OUTBYTES];
    B2_RES(blake2s(hash_pay, sizeof(hash_pay), pay, pay_len, NULL, 0));
    B2_RES(blake2s_update(&b2s, hash_pay, sizeof(hash_pay)));
    B2_RES(blake2s_final(&b2s, &dg.fp, sizeof(dg.fp)));

    /* Serialize datagram into caller's buffer and return serialized size. */
    struct cursor csr = cursor_new(out, *out_len);
    SER_RES(lfc_dg_monolithic__ser(&dg, &csr));
    *out_len = csr.pos;
    return lfc_res_ok;
}


uint32_t
lfc_seq(struct lfc * lfc) {
    return lfc->seq++;
}
