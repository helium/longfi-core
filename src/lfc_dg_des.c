#include "lfc/priv/lfc_dg_des.h"
#include "cursor/varint.h"
#include "golay/golay.h"
#include <assert.h>
#include <stdbool.h>

#define CSR_RES(eXpr_)                                                         \
    do {                                                                       \
        enum cursor_res vAl_ = (eXpr_);                                        \
        if (cursor_res_err_buf_exhausted == (vAl_)) {                          \
            return lfc_dg_des_res_err_nomem;                                   \
        }                                                                      \
        assert(vAl_ == cursor_res_ok);                                         \
    } while (0)

#define LFC_RES(eXpr_)                                                         \
    do {                                                                       \
        enum lfc_dg_des_res vAl_ = (eXpr_);                                    \
        if (vAl_ != lfc_dg_des_res_ok) {                                       \
            return res;                                                        \
        }                                                                      \
    } while (0)




enum lfc_dg_des_res
lfc_dg__des(struct lfc_dg_des * out, struct cursor * csr) {
    enum lfc_dg_des_res res;
    struct lfc_dg_hdr   hdr;

    res = lfc_dg_hdr__des(&hdr, csr);
    LFC_RES(res);

    switch (hdr.type) {
    case lfc_dg_type_monolithic:
        res = lfc_dg_monolithic__des(&out->monolithic, hdr, csr);
        LFC_RES(res);
        break;

    case lfc_dg_type_ack:
        res = lfc_dg_ack__des(&out->ack, hdr, csr);
        LFC_RES(res);
        break;

    case lfc_dg_type_frame_start:
        res = lfc_dg_frame_start__des(&out->frame_start, hdr, csr);
        LFC_RES(res);
        break;

    case lfc_dg_type_frame_data:
        res = lfc_dg_frame_data__des(&out->frame_data, hdr, csr);
        LFC_RES(res);
        break;
    };

    out->type = hdr.type;

    assert(res == lfc_dg_des_res_ok);
    return res;
}


enum lfc_dg_des_res
lfc_dg_monolithic__des(struct lfc_dg_monolithic * out,
                       struct lfc_dg_hdr          hdr,
                       struct cursor *            csr) {
    assert(hdr.type == lfc_dg_type_monolithic);

    out->flags = lfc_dg_monolithic_flags__des(hdr.flag_bits);
    CSR_RES(cursor_unpack_var_le_u32(csr, &out->oui));
    CSR_RES(cursor_unpack_var_le_u32(csr, &out->did));
    CSR_RES(cursor_unpack_le_u32(csr, &out->fp));
    CSR_RES(cursor_unpack_var_le_u32(csr, &out->seq));
    out->pay_len = cursor_take_remaining(csr, out->pay);

    return lfc_dg_des_res_ok;
}


enum lfc_dg_des_res
lfc_dg_ack__des(struct lfc_dg_ack * out,
                struct lfc_dg_hdr   hdr,
                struct cursor *     csr) {
    assert(hdr.type == lfc_dg_type_ack);

    out->flags = lfc_dg_ack_flags__des(hdr.flag_bits);
    CSR_RES(cursor_unpack_var_le_u32(csr, &out->oui));
    CSR_RES(cursor_unpack_var_le_u32(csr, &out->did));
    CSR_RES(cursor_unpack_le_u32(csr, &out->fp));
    CSR_RES(cursor_unpack_var_le_u32(csr, &out->seq));
    out->pay_len = cursor_take_remaining(csr, out->pay);

    return lfc_dg_des_res_ok;
}


enum lfc_dg_des_res
lfc_dg_frame_start__des(struct lfc_dg_frame_start * out,
                        struct lfc_dg_hdr           hdr,
                        struct cursor *             csr) {
    assert(hdr.type == lfc_dg_type_frame_start);

    out->flags = lfc_dg_frame_start_flags__des(hdr.flag_bits);
    CSR_RES(cursor_unpack_var_le_u32(csr, &out->oui));
    CSR_RES(cursor_unpack_var_le_u32(csr, &out->did));
    CSR_RES(cursor_unpack_le_u32(csr, &out->fp));
    CSR_RES(cursor_unpack_var_le_u32(csr, &out->seq));
    CSR_RES(cursor_unpack_var_le_u32(csr, &out->fragments));
    out->pay_len = cursor_take_remaining(csr, out->pay);

    return lfc_dg_des_res_ok;
}


enum lfc_dg_des_res
lfc_dg_frame_data__des(struct lfc_dg_frame_data * out,
                       struct lfc_dg_hdr          hdr,
                       struct cursor *            csr) {
    assert(hdr.type == lfc_dg_type_frame_data);

    out->flags = lfc_dg_frame_data_flags__des(hdr.flag_bits);
    CSR_RES(cursor_unpack_var_le_u32(csr, &out->oui));
    CSR_RES(cursor_unpack_var_le_u32(csr, &out->did));
    CSR_RES(cursor_unpack_le_u32(csr, &out->fp));
    CSR_RES(cursor_unpack_var_le_u32(csr, &out->fragment));
    out->pay_len = cursor_take_remaining(csr, out->pay);

    return lfc_dg_des_res_ok;
}


struct lfc_dg_monolithic_flags
lfc_dg_monolithic_flags__des(uint8_t flag_bits) {
    return (struct lfc_dg_monolithic_flags){
        .downlink   = ((flag_bits >> 0) & 1) == 1,
        .should_ack = ((flag_bits >> 1) & 1) == 1,
        .cts_rts    = ((flag_bits >> 2) & 1) == 1,
        .priority   = ((flag_bits >> 3) & 1) == 1,
        .ldpc       = ((flag_bits >> 4) & 1) == 1,
    };
}


struct lfc_dg_frame_start_flags
lfc_dg_frame_start_flags__des(uint8_t flag_bits) {
    return (struct lfc_dg_frame_start_flags){
        .downlink   = ((flag_bits >> 0) & 1) == 1,
        .should_ack = ((flag_bits >> 1) & 1) == 1,
        .cts_rts    = ((flag_bits >> 2) & 1) == 1,
        .priority   = ((flag_bits >> 3) & 1) == 1,
        .ldpc       = ((flag_bits >> 4) & 1) == 1,
    };
}


struct lfc_dg_frame_data_flags
lfc_dg_frame_data_flags__des(uint8_t flag_bits) {
    return (struct lfc_dg_frame_data_flags){
        .ldpc = (flag_bits & 1) == 1,
    };
}


struct lfc_dg_ack_flags
lfc_dg_ack_flags__des(uint8_t flag_bits) {
    return (struct lfc_dg_ack_flags){
        .failure         = ((flag_bits >> 0) & 1) == 1,
        .session_expired = ((flag_bits >> 1) & 1) == 1,
        .cts_rts         = ((flag_bits >> 2) & 1) == 1,
        .retransmit      = ((flag_bits >> 3) & 1) == 1,
        .ldpc            = ((flag_bits >> 4) & 1) == 1,
    };
}


enum lfc_dg_des_res
lfc_dg_type__des(enum lfc_dg_type * out, uint8_t type_bits) {
    if (type_bits == 0 || type_bits > lfc_dg_type_ack) {
        return lfc_dg_des_res_invalid_type;
    }
    *out = (enum lfc_dg_type)type_bits;
    return lfc_dg_des_res_ok;
}


enum lfc_dg_des_res
lfc_dg_hdr__des(struct lfc_dg_hdr * out, struct cursor * csr) {
    enum lfc_dg_des_res res;

    uint16_t dec_hdr_bits;
    uint8_t  enc_hdr_bits[3];
    CSR_RES(cursor_take(csr, sizeof(enc_hdr_bits), enc_hdr_bits));

    int     dec_bit_errs    = golay_decode12(enc_hdr_bits, &dec_hdr_bits);
    bool    dg_extended_bit = 1 == ((dec_hdr_bits >> 12) & 1);
    uint8_t dg_type_bits    = (dec_hdr_bits >> 6 & 0x1F /* 0b1_1111 */);
    uint8_t dg_flags_bits   = (dec_hdr_bits & 0x3F /* 0b11_1111 */);

    /* We're not handling extended tags yet. */
    assert(!dg_extended_bit);

    *out = (struct lfc_dg_hdr){
        .bit_errs  = dec_bit_errs,
        .flag_bits = dg_flags_bits,
    };

    res = lfc_dg_type__des(&out->type, dg_type_bits);
    LFC_RES(res);

    return lfc_dg_des_res_ok;
}
