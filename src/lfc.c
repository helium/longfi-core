#include "lfc/lfc.h"
#include "blake2.h"
#include "golay/golay.h"
#include "lfc/datagram.h"
#include "lfc/fingerprint.h"
#include "lfc/priv/lfc_dg_des.h"
#include "lfc/priv/lfc_dg_ser.h"
#include <assert.h>
#include <string.h>

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

/* Returns and increments sequence number. */
static uint32_t
lfc_seq(struct lfc * lfc);


void
lfc_init(struct lfc * lfc, struct lfc_user_cfg cfg) {
    *lfc = (struct lfc){
        .seq = 0,
        .cfg = cfg,
    };
}


enum lfc_res
lfc_dg_monolithic__fingerprint(struct lfc const *               lfc,
                               struct lfc_dg_monolithic const * dg,
                               uint32_t *                       out) {
    /* Assuming no set flags for this PoC. */
    uint8_t  flag_bits = 0;
    uint16_t hdr       = (lfc_dg_type_monolithic << 6) | flag_bits;
    return lfc_fingerprint_monolithic(lfc->cfg.key,
                                      lfc->cfg.key_len,
                                      hdr,
                                      dg->oui,
                                      dg->did,
                                      dg->seq,
                                      dg->pay,
                                      dg->pay_len,
                                      out);
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
                                           .downlink = lfc->cfg.personality
                                                       == lfc_personality_router,
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

    uint32_t fp = 42;
    lfc_dg_monolithic__fingerprint(lfc, &dg, &fp);
    dg.fp = fp;

    struct cursor csr = cursor_new(out, *out_len);
    LFC_RES(lfc_dg_monolithic__ser(&dg, &csr));
    *out_len = csr.pos;
    return lfc_res_ok;
}


enum lfc_res
lfc_receive(struct lfc const * lfc,
            uint8_t const *    in,
            size_t             in_len,
            uint8_t *          out,
            size_t *           out_len) {
    size_t out_cap = *out_len;
    *out_len       = 0;

    struct cursor     csr = cursor_new((uint8_t *)in, in_len);
    struct lfc_dg_des dg;
    LFC_RES(lfc_dg__des(&dg, &csr));

    if (dg.type != lfc_dg_type_monolithic) {
        return lfc_res_invalid_type;
    }

    if ((dg.monolithic.flags.downlink
         && lfc->cfg.personality == lfc_personality_router)
        || (!dg.monolithic.flags.downlink
            && lfc->cfg.personality == lfc_personality_device)) {
        return lfc_res_err_addr;
    }

    if (out_cap < dg.monolithic.pay_len) {
        return lfc_res_err_nomem;
    }

    memmove(out, dg.monolithic.pay, dg.monolithic.pay_len);
    *out_len = dg.monolithic.pay_len;

    uint32_t fp;
    LFC_RES(lfc_dg_monolithic__fingerprint(lfc, &dg.monolithic, &fp));

    if ((dg.monolithic.oui != lfc->cfg.oui)
        || (dg.monolithic.did != lfc->cfg.did)) {
        return lfc_res_err_addr;
    }

    if (fp != dg.monolithic.fp) {
        return lfc_res_err_fingerprint;
    }

    return lfc_res_ok;
}


static uint32_t
lfc_seq(struct lfc * lfc) {
    return lfc->seq++;
}
