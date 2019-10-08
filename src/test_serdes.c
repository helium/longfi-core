#include "greatest/greatest.h"
#include "lfc/datagram.h"
#include "lfc/priv/lfc_dg_des.h"
#include "lfc/priv/lfc_dg_eq.h"
#include "lfc/priv/lfc_dg_ser.h"
#include <assert.h>
#include <limits.h>
#include <stdbool.h>

uint32_t
ru32() {
    return arc4random();
}

bool
bit(uint32_t in, size_t idx) {
    assert((idx / CHAR_BIT) < sizeof(in));
    return (in >> idx) & 1;
}

void
gen_payload(void * dst, size_t * dst_len_tag) {
    memset(dst, 0, LFC_DG_CONSTANTS_MAX_PAY_LEN);
    *dst_len_tag = arc4random_uniform(LFC_DG_CONSTANTS_MAX_PAY_LEN);
    arc4random_buf(dst, *dst_len_tag);
}

#define GEN_GEN_FLAGS(DG_TYPE)                                                 \
    void gen_##DG_TYPE##_flags(struct lfc_dg_##DG_TYPE##_flags * out) {        \
        uint32_t bits   = ru32();                                              \
        out->downlink   = bit(bits, 0);                                        \
        out->should_ack = bit(bits, 1);                                        \
        out->cts_rts    = bit(bits, 2);                                        \
        out->priority   = bit(bits, 3);                                        \
        out->ldpc       = bit(bits, 4);                                        \
    }

GEN_GEN_FLAGS(frame_start)
GEN_GEN_FLAGS(monolithic)

void
gen_monolithic(struct lfc_dg_monolithic * out) {
    gen_monolithic_flags(&out->flags);
    out->oui = ru32();
    out->did = ru32();
    out->fp  = ru32();
    out->seq = ru32();
    gen_payload(out->pay, &out->pay_len);
}

void
gen_ack_flags(struct lfc_dg_ack_flags * out) {
    uint32_t bits        = ru32();
    out->failure         = bit(bits, 0);
    out->session_expired = bit(bits, 1);
    out->cts_rts         = bit(bits, 2);
    out->retransmit      = bit(bits, 3);
    out->ldpc            = bit(bits, 4);
}

void
gen_ack(struct lfc_dg_ack * out) {
    gen_ack_flags(&out->flags);
    out->oui = ru32();
    out->did = ru32();
    out->fp  = ru32();
    out->seq = ru32();
    gen_payload(out->pay, &out->pay_len);
}

void
gen_frame_start(struct lfc_dg_frame_start * out) {
    out->oui = ru32();
    out->did = ru32();
    out->fp  = ru32();
    out->seq = ru32();
    gen_payload(out->pay, &out->pay_len);
}

void
gen_frame_data_flags(struct lfc_dg_frame_data_flags * out) {
    out->ldpc = bit(ru32(), 0);
}

void
gen_frame_data(struct lfc_dg_frame_data * out) {
    gen_frame_data_flags(&out->flags);
    out->oui      = ru32();
    out->did      = ru32();
    out->fp       = ru32();
    out->fragment = ru32();
    gen_payload(out->pay, &out->pay_len);
}

#define GEN_TEST(DG_TYPE)                                                      \
    TEST test_##DG_TYPE##_roundtrips() {                                       \
        /* Generate random reference. */                                       \
        struct lfc_dg_##DG_TYPE REF;                                           \
        gen_##DG_TYPE(&REF);                                                   \
                                                                               \
        /* Serialize reference. */                                             \
        uint8_t             buf_ser[1024];                                     \
        struct cursor       csr_ser = cursor_new(buf_ser, sizeof(buf_ser));    \
        enum lfc_dg_ser_res sres    = lfc_dg_##DG_TYPE##__ser(&REF, &csr_ser); \
        ASSERT_EQ(lfc_dg_ser_res_ok, sres);                                    \
                                                                               \
        /* Deserialize. */                                                     \
        struct cursor       csr_des = cursor_new(buf_ser, csr_ser.pos);        \
        struct lfc_dg_des   deserialized;                                      \
        enum lfc_dg_des_res dres = lfc_dg__des(&deserialized, &csr_des);       \
        ASSERT_EQ(lfc_dg_ser_res_ok, dres);                                    \
        ASSERT_EQ_FMT(lfc_dg_type_##DG_TYPE, deserialized.type, "%x");         \
        ASSERT(lfc_dg_##DG_TYPE##__eq(&REF, &deserialized.DG_TYPE));           \
                                                                               \
        /* Reserialize. */                                                     \
        uint8_t       buf_reser[1024];                                         \
        struct cursor csr_reser = cursor_new(buf_reser, sizeof(buf_reser));    \
        sres = lfc_dg_##DG_TYPE##__ser(&deserialized.DG_TYPE, &csr_reser);     \
        ASSERT_EQ(lfc_dg_ser_res_ok, sres);                                    \
                                                                               \
        /* Compare serilized and reserialized. */                              \
        ASSERT_EQ_FMT(csr_ser.pos, csr_reser.pos, "%d");                       \
        ASSERT_MEM_EQ(csr_ser.buf, csr_reser.buf, csr_ser.pos);                \
                                                                               \
        PASS();                                                                \
    }

GEN_TEST(ack);
GEN_TEST(frame_data);
GEN_TEST(frame_start);
GEN_TEST(monolithic);

GREATEST_MAIN_DEFS();

int
main(int argc, char * argv[]) {
    GREATEST_MAIN_BEGIN();
    for (int i = 0; i < 100; ++i) {
        RUN_TEST(test_ack_roundtrips);
        RUN_TEST(test_frame_data_roundtrips);
        RUN_TEST(test_frame_start_roundtrips);
        RUN_TEST(test_monolithic_roundtrips);
    }
    GREATEST_MAIN_END();
}
