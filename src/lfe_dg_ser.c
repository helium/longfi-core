#include "lfe_dg_ser.h"
#include "cursor/varint.h"
#include "golay/golay.h"
#include <assert.h>

#define CSR_RES(eXpr_)                                                         \
    do {                                                                       \
        enum cursor_res vAl_ = (eXpr_);                                        \
        if (cursor_res_err_buf_exhausted == (vAl_)) {                          \
            return lfe_dg_ser_res_err_nomem;                                   \
        }                                                                      \
        assert(vAl_ == cursor_res_ok);                                         \
    } while (0)

#define LFE_RES(eXpr_)                                                         \
    do {                                                                       \
        enum lfe_dg_ser_res vAl_ = (eXpr_);                                    \
        if (vAl_ != lfe_dg_ser_res_ok) {                                       \
            return vAl_;                                                       \
        }                                                                      \
    } while (0)


enum lfe_dg_ser_res
lfe_dg_hdr__ser(enum lfe_dg_type type, uint8_t flag_bits, struct cursor * csr) {
    assert(!(flag_bits & 0xC0));
    uint16_t hdr = (type << 6) | flag_bits;
    uint8_t  encoded[3];
    golay_encode12(hdr, encoded);
    CSR_RES(cursor_put(csr, encoded, sizeof(encoded)));
    return lfe_dg_ser_res_ok;
}


enum lfe_dg_ser_res
lfe_dg_monolithic__ser(struct lfe_dg_monolithic const * in, struct cursor * csr) {
    LFE_RES(lfe_dg_hdr__ser(lfe_dg_type_monolithic,
                            lfe_dg_monolithic_flags__ser(&in->flags),
                            csr));
    CSR_RES(cursor_pack_var_le_u32(csr, in->oui));
    CSR_RES(cursor_pack_var_le_u32(csr, in->did));
    CSR_RES(cursor_pack_le_u32(csr, in->fp));
    CSR_RES(cursor_pack_var_le_u32(csr, in->seq));
    CSR_RES(cursor_put(csr, in->pay, in->pay_len));
    return lfe_dg_ser_res_ok;
}


enum lfe_dg_ser_res
lfe_dg_ack__ser(struct lfe_dg_ack const * in, struct cursor * csr) {
    LFE_RES(
        lfe_dg_hdr__ser(lfe_dg_type_ack, lfe_dg_ack_flags__ser(&in->flags), csr));
    CSR_RES(cursor_pack_var_le_u32(csr, in->oui));
    CSR_RES(cursor_pack_var_le_u32(csr, in->did));
    CSR_RES(cursor_pack_le_u32(csr, in->fp));
    CSR_RES(cursor_pack_var_le_u32(csr, in->seq));
    CSR_RES(cursor_put(csr, in->pay, in->pay_len));
    return lfe_dg_ser_res_ok;
}


enum lfe_dg_ser_res
lfe_dg_frame_start__ser(struct lfe_dg_frame_start const * in,
                        struct cursor *                   csr) {
    LFE_RES(lfe_dg_hdr__ser(lfe_dg_type_frame_start,
                            lfe_dg_frame_start_flags__ser(&in->flags),
                            csr));
    CSR_RES(cursor_pack_var_le_u32(csr, in->oui));
    CSR_RES(cursor_pack_var_le_u32(csr, in->did));
    CSR_RES(cursor_pack_le_u32(csr, in->fp));
    CSR_RES(cursor_pack_var_le_u32(csr, in->seq));
    CSR_RES(cursor_pack_var_le_u32(csr, in->fragments));
    CSR_RES(cursor_put(csr, in->pay, in->pay_len));
    return lfe_dg_ser_res_ok;
}


enum lfe_dg_ser_res
lfe_dg_frame_data__ser(struct lfe_dg_frame_data const * in, struct cursor * csr) {
    LFE_RES(lfe_dg_hdr__ser(lfe_dg_type_frame_data,
                            lfe_dg_frame_data_flags__ser(&in->flags),
                            csr));
    CSR_RES(cursor_pack_var_le_u32(csr, in->oui));
    CSR_RES(cursor_pack_var_le_u32(csr, in->did));
    CSR_RES(cursor_pack_le_u32(csr, in->fp));
    CSR_RES(cursor_pack_var_le_u32(csr, in->fragment));
    CSR_RES(cursor_put(csr, in->pay, in->pay_len));
    return lfe_dg_ser_res_ok;
}


uint8_t
lfe_dg_monolithic_flags__ser(struct lfe_dg_monolithic_flags const * flags) {
    return flags->downlink << 0 | flags->should_ack << 1 | flags->cts_rts << 2
           | flags->priority << 3 | flags->ldpc << 4;
}


uint8_t
lfe_dg_frame_start_flags__ser(struct lfe_dg_frame_start_flags const * flags) {
    return flags->downlink << 0 | flags->should_ack << 1 | flags->cts_rts << 2
           | flags->priority << 3 | flags->ldpc << 4;
}


uint8_t
lfe_dg_frame_data_flags__ser(struct lfe_dg_frame_data_flags const * flags) {
    return flags->ldpc;
}


uint8_t
lfe_dg_ack_flags__ser(struct lfe_dg_ack_flags const * flags) {
    return (flags->failure << 0) | (flags->session_expired << 1)
           | (flags->cts_rts << 2) | (flags->retransmit << 3)
           | (flags->ldpc << 4);
}
