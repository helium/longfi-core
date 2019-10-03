#include "lfe_dg_parse.h"
#include "golay/golay.h"
#include <assert.h>
#include <stdbool.h>

#define CSR_RES(_eXpr_)                                                        \
    do {                                                                       \
        if (cursor_res_err_buf_exhausted == (_eXpr_)) {                        \
            return lfe_dg_parse_res_err_nomem;                                 \
        }                                                                      \
    } while (0)

enum lfe_dg_parse_res
lfe_dg__parse(struct lfe_dg_parsed * out, struct cursor * csr) {
    return lfe_dg_parse_res_ok;
}

struct lfe_dg_flags
lfe_dg_parse__flags(uint8_t flag_bits) {
    return (struct lfe_dg_flags){
        .failure         = ((flag_bits >> 0) & 1) == 1,
        .session_expired = ((flag_bits >> 1) & 1) == 1,
        .cts_rts         = ((flag_bits >> 2) & 1) == 1,
        .retransmit      = ((flag_bits >> 3) & 1) == 1,
        .ldpc            = ((flag_bits >> 4) & 1) == 1,
        .reserved        = ((flag_bits >> 5) & 1) == 1,
    };
}

enum lfe_dg_parse_res
lfe_dg_type__parse(enum lfe_dg_type * out, uint8_t type_bits) {
    if (type_bits == 0 || type_bits > lfe_dg_type_ack) {
        return lfe_dg_parse_res_invalid_type;
    }
    *out = (enum lfe_dg_type)type_bits;
    return lfe_dg_parse_res_ok;
}

enum lfe_dg_parse_res
lfe_dg_hdr__parse(struct lfe_dg_hdr * out, struct cursor * csr) {
    enum lfe_dg_parse_res res;

    uint16_t dec_hdr_bits;
    uint8_t  enc_hdr_bits[3];
    CSR_RES(cursor_take(csr, sizeof(enc_hdr_bits), enc_hdr_bits));

    int     dec_bit_errs    = golay_decode12(enc_hdr_bits, &dec_hdr_bits);
    bool    dg_extended_bit = 1 == ((dec_hdr_bits >> 12) & 1);
    uint8_t dg_type_bits    = (dec_hdr_bits >> 6 & 0x1F /* 0b1_1111 */);
    uint8_t dg_flags_bits   = (dec_hdr_bits & 0x3F /* 0b11_1111 */);

    /* We're not handling extended tags yet. */
    assert(!dg_extended_bit);

    *out = (struct lfe_dg_hdr){
        .bit_errs  = dec_bit_errs,
        .flag_bits = dg_flags_bits,
    };

    if ((res = lfe_dg_type__parse(&out->type, dg_type_bits))) {
        return res;
    }

    return lfe_dg_parse_res_ok;
}
