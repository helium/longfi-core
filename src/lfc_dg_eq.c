#include "lfc_dg_eq.h"
#include <string.h>

/* Member EQuality. */
#define MEQ(MEMBER)                                                            \
    do {                                                                       \
        if (l->MEMBER != r->MEMBER)                                            \
            return false;                                                      \
    } while (0)

/* Payload EQuality. */
#define PEQ()                                                                  \
    do {                                                                       \
        if (l->pay_len != r->pay_len) {                                        \
            return false;                                                      \
        } else if (memcmp(l->pay, r->pay, l->pay_len)) {                       \
            return false;                                                      \
        }                                                                      \
    } while (0)

bool
lfc_dg_monolithic__eq(struct lfc_dg_monolithic const * l,
                      struct lfc_dg_monolithic const * r) {
    MEQ(flags.downlink);
    MEQ(flags.should_ack);
    MEQ(flags.cts_rts);
    MEQ(flags.priority);
    MEQ(flags.ldpc);
    MEQ(oui);
    MEQ(did);
    MEQ(fp);
    MEQ(seq);
    MEQ(pay_len);
    PEQ();
    return true;
}

bool
lfc_dg_frame_start__eq(struct lfc_dg_frame_start const * l,
                       struct lfc_dg_frame_start const * r) {
    MEQ(flags.downlink);
    MEQ(flags.should_ack);
    MEQ(flags.cts_rts);
    MEQ(flags.priority);
    MEQ(flags.ldpc);
    MEQ(oui);
    MEQ(did);
    MEQ(fp);
    MEQ(seq);
    MEQ(fragments);
    PEQ();
    return true;
}

bool
lfc_dg_frame_data__eq(struct lfc_dg_frame_data const * l,
                      struct lfc_dg_frame_data const * r) {
    MEQ(flags.ldpc);
    MEQ(oui);
    MEQ(did);
    MEQ(fp);
    MEQ(fragment);
    PEQ();
    return true;
}

bool
lfc_dg_ack__eq(struct lfc_dg_ack const * l, struct lfc_dg_ack const * r) {
    MEQ(flags.failure);
    MEQ(flags.session_expired);
    MEQ(flags.cts_rts);
    MEQ(flags.retransmit);
    MEQ(flags.ldpc);
    MEQ(oui);
    MEQ(did);
    MEQ(fp);
    MEQ(seq);
    PEQ();
    return true;
}
