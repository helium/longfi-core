#include "lfc/priv/lfc_dg_ser.h"
#include "cursor/varint.h"
#include "golay/golay.h"
#include <assert.h>

#define CSR_RES(eXpr_)                                                         \
    do {                                                                       \
        enum cursor_res vAl_ = (eXpr_);                                        \
        if (cursor_res_err_buf_exhausted == (vAl_)) {                          \
            return lfc_res_err_nomem;                                          \
        }                                                                      \
        assert(vAl_ == cursor_res_ok);                                         \
    } while (0)

#define LFC_RES(eXpr_)                                                         \
    do {                                                                       \
        enum lfc_res vAl_ = (eXpr_);                                           \
        if (vAl_ != lfc_res_ok) {                                              \
            return vAl_;                                                       \
        }                                                                      \
    } while (0)


enum lfc_res
lfc_dg_hdr__ser(enum lfc_dg_type type, uint8_t flag_bits, struct cursor * csr) {
    assert(!(flag_bits & 0xC0));
    uint16_t hdr = (type << 6) | flag_bits;
    uint8_t  encoded[3];
    golay_encode12(hdr, encoded);
    CSR_RES(cursor_put(csr, encoded, sizeof(encoded)));
    return lfc_res_ok;
}


enum lfc_res
lfc_dg_monolithic__ser(struct lfc_dg_monolithic const * in, struct cursor * csr) {
    LFC_RES(lfc_dg_hdr__ser(lfc_dg_type_monolithic,
                            lfc_dg_monolithic_flags__ser(&in->flags),
                            csr));
    CSR_RES(cursor_pack_var_le_u32(csr, in->oui));
    CSR_RES(cursor_pack_var_le_u32(csr, in->did));
    CSR_RES(cursor_pack_le_u32(csr, in->fp));
    CSR_RES(cursor_pack_var_le_u32(csr, in->seq));
    CSR_RES(cursor_put(csr, in->pay, in->pay_len));
    return lfc_res_ok;
}


enum lfc_res
lfc_dg_ack__ser(struct lfc_dg_ack const * in, struct cursor * csr) {
    LFC_RES(
        lfc_dg_hdr__ser(lfc_dg_type_ack, lfc_dg_ack_flags__ser(&in->flags), csr));
    CSR_RES(cursor_pack_var_le_u32(csr, in->oui));
    CSR_RES(cursor_pack_var_le_u32(csr, in->did));
    CSR_RES(cursor_pack_le_u32(csr, in->fp));
    CSR_RES(cursor_pack_var_le_u32(csr, in->seq));
    CSR_RES(cursor_put(csr, in->pay, in->pay_len));
    return lfc_res_ok;
}


enum lfc_res
lfc_dg_frame_start__ser(struct lfc_dg_frame_start const * in,
                        struct cursor *                   csr) {
    LFC_RES(lfc_dg_hdr__ser(lfc_dg_type_frame_start,
                            lfc_dg_frame_start_flags__ser(&in->flags),
                            csr));
    CSR_RES(cursor_pack_var_le_u32(csr, in->oui));
    CSR_RES(cursor_pack_var_le_u32(csr, in->did));
    CSR_RES(cursor_pack_le_u32(csr, in->fp));
    CSR_RES(cursor_pack_var_le_u32(csr, in->seq));
    CSR_RES(cursor_pack_var_le_u32(csr, in->fragments));
    CSR_RES(cursor_put(csr, in->pay, in->pay_len));
    return lfc_res_ok;
}


enum lfc_res
lfc_dg_frame_data__ser(struct lfc_dg_frame_data const * in, struct cursor * csr) {
    LFC_RES(lfc_dg_hdr__ser(lfc_dg_type_frame_data,
                            lfc_dg_frame_data_flags__ser(&in->flags),
                            csr));
    CSR_RES(cursor_pack_var_le_u32(csr, in->oui));
    CSR_RES(cursor_pack_var_le_u32(csr, in->did));
    CSR_RES(cursor_pack_le_u32(csr, in->fp));
    CSR_RES(cursor_pack_var_le_u32(csr, in->fragment));
    CSR_RES(cursor_put(csr, in->pay, in->pay_len));
    return lfc_res_ok;
}


uint8_t
lfc_dg_monolithic_flags__ser(struct lfc_dg_monolithic_flags const * flags) {
    return flags->downlink << 0 | flags->should_ack << 1 | flags->cts_rts << 2
           | flags->priority << 3 | flags->ldpc << 4;
}


uint8_t
lfc_dg_frame_start_flags__ser(struct lfc_dg_frame_start_flags const * flags) {
    return flags->downlink << 0 | flags->should_ack << 1 | flags->cts_rts << 2
           | flags->priority << 3 | flags->ldpc << 4;
}


uint8_t
lfc_dg_frame_data_flags__ser(struct lfc_dg_frame_data_flags const * flags) {
    return flags->ldpc;
}


uint8_t
lfc_dg_ack_flags__ser(struct lfc_dg_ack_flags const * flags) {
    return (flags->failure << 0) | (flags->session_expired << 1)
           | (flags->cts_rts << 2) | (flags->retransmit << 3)
           | (flags->ldpc << 4);
}
