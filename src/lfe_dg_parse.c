#include "lfe_dg_parse.h"
#include "cursor/varint.h"
#include "golay/golay.h"
#include <assert.h>
#include <stdbool.h>

#define CSR_RES(eXpr_)                                                         \
    do {                                                                       \
        enum cursor_res vAl_ = (eXpr_);                                        \
        if (cursor_res_err_buf_exhausted == (vAl_)) {                          \
            return lfe_dg_parse_res_err_nomem;                                 \
        }                                                                      \
        assert(vAl_ == cursor_res_ok);                                         \
    } while (0)

#define LFE_RES(eXpr_)                                                         \
    do {                                                                       \
        enum lfe_dg_parse_res vAl_ = (eXpr_);                                  \
        if (vAl_ != lfe_dg_parse_res_ok) {                                     \
            return res;                                                        \
        }                                                                      \
    } while (0)




enum lfe_dg_parse_res
lfe_dg__parse(struct lfe_dg_parsed * out, struct cursor * csr) {
    enum lfe_dg_parse_res res;
    struct lfe_dg_hdr     hdr;

    res = lfe_dg_hdr__parse(&hdr, csr);
    LFE_RES(res);

    switch (hdr.type) {
    case lfe_dg_type_monolithic:
        res = lfe_dg_monolithic__parse(&out->monolithic, hdr, csr);
        LFE_RES(res);
        break;

    case lfe_dg_type_ack:
        res = lfe_dg_ack__parse(&out->ack, hdr, csr);
        LFE_RES(res);
        break;

    case lfe_dg_type_frame_start:
        res = lfe_dg_frame_start__parse(&out->frame_start, hdr, csr);
        LFE_RES(res);
        break;

    case lfe_dg_type_frame_data:
        res = lfe_dg_frame_data__parse(&out->frame_data, hdr, csr);
        LFE_RES(res);
        break;
    };

    assert(res == lfe_dg_parse_res_ok);
    return res;
}


enum lfe_dg_parse_res
lfe_dg_monolithic__parse(struct lfe_dg_monolithic * out,
                         struct lfe_dg_hdr          hdr,
                         struct cursor *            csr) {
    assert(hdr.type = lfe_dg_type_monolithic);

    out->flags = lfe_dg_monolithic_flags__parse(hdr.flag_bits);
    CSR_RES(cursor_unpack_var_le_u32(csr, &out->oui));
    CSR_RES(cursor_unpack_var_le_u32(csr, &out->did));
    CSR_RES(cursor_unpack_le_u32(csr, &out->fp));
    CSR_RES(cursor_unpack_var_le_u32(csr, &out->seq));
    out->pay_len = cursor_take_remaining(csr, out->pay);

    return lfe_dg_parse_res_ok;
}


enum lfe_dg_parse_res
lfe_dg_ack__parse(struct lfe_dg_ack * out,
                  struct lfe_dg_hdr   hdr,
                  struct cursor *     csr) {
    assert(hdr.type = lfe_dg_type_ack);
    enum lfe_dg_parse_res res = lfe_dg_parse_res_ok;
    assert(res == lfe_dg_parse_res_ok);
    return res;
}


enum lfe_dg_parse_res
lfe_dg_frame_start__parse(struct lfe_dg_frame_start * out,
                          struct lfe_dg_hdr           hdr,
                          struct cursor *             csr) {
    assert(hdr.type = lfe_dg_type_frame_start);
    enum lfe_dg_parse_res res = lfe_dg_parse_res_ok;
    assert(res == lfe_dg_parse_res_ok);
    return res;
}


enum lfe_dg_parse_res
lfe_dg_frame_data__parse(struct lfe_dg_frame_data * out,
                         struct lfe_dg_hdr          hdr,
                         struct cursor *            csr) {
    assert(hdr.type = lfe_dg_type_frame_data);
    enum lfe_dg_parse_res res = lfe_dg_parse_res_ok;
    assert(res == lfe_dg_parse_res_ok);
    return res;
}


struct lfe_dg_monolithic_flags
lfe_dg_monolithic_flags__parse(uint8_t flag_bits) {
    return (struct lfe_dg_monolithic_flags){
        .downlink   = ((flag_bits >> 0) & 1) == 1,
        .should_ack = ((flag_bits >> 1) & 1) == 1,
        .cts_rts    = ((flag_bits >> 2) & 1) == 1,
        .priority   = ((flag_bits >> 3) & 1) == 1,
        .ldpc       = ((flag_bits >> 4) & 1) == 1,
    };
}


struct lfe_dg_ack_flags
lfe_dg_ack_flags__parse(uint8_t flag_bits) {
    return (struct lfe_dg_ack_flags){
        .failure         = ((flag_bits >> 0) & 1) == 1,
        .session_expired = ((flag_bits >> 1) & 1) == 1,
        .cts_rts         = ((flag_bits >> 2) & 1) == 1,
        .retransmit      = ((flag_bits >> 3) & 1) == 1,
        .ldpc            = ((flag_bits >> 4) & 1) == 1,
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

    res = lfe_dg_type__parse(&out->type, dg_type_bits);
    LFE_RES(res);

    return lfe_dg_parse_res_ok;
}
