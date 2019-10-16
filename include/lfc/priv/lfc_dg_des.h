#ifndef LFC_DG_DES_25E7091F
#define LFC_DG_DES_25E7091F

#include "cursor/cursor.h"
#include "lfc/datagram.h"
#include "lfc/lfc.h"
#include <stdbool.h>

/**
 * @file
 *
 * Private API for parsing received datagrams.
 *
 * @addtogroup Private
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif


/**
 * A successfully parsed datagram.
 */
struct lfc_dg_des {
    /**
     * Number of corrected bit errors in the header.
     */
    size_t hdr_bit_errs;
    /**
     * This datagram's type.
     *
     * Indicates which member of `u` is valid.
     */
    enum lfc_dg_type type;
    union {
        struct lfc_dg_monolithic  monolithic;
        struct lfc_dg_frame_start frame_start;
        struct lfc_dg_frame_data  frame_data;
        struct lfc_dg_ack         ack;
    };
};


/**
 * Parses a received packet.
 */
enum lfc_res
lfc_dg__des(struct lfc_dg_des * out, struct cursor * csr);


/**
 * Parses a `monolithic` datagram.
 */
enum lfc_res
lfc_dg_monolithic__des(struct lfc_dg_monolithic * out,
                       struct lfc_dg_hdr          hdr,
                       struct cursor *            csr);


/**
 * Parses a `ack` datagram.
 */
enum lfc_res
lfc_dg_ack__des(struct lfc_dg_ack * out,
                struct lfc_dg_hdr   hdr,
                struct cursor *     csr);


/**
 * Parses a `frame_start` datagram.
 */
enum lfc_res
lfc_dg_frame_start__des(struct lfc_dg_frame_start * out,
                        struct lfc_dg_hdr           hdr,
                        struct cursor *             csr);


/**
 * Parses a `frame_data` datagram.
 */
enum lfc_res
lfc_dg_frame_data__des(struct lfc_dg_frame_data * out,
                       struct lfc_dg_hdr          hdr,
                       struct cursor *            csr);


/**
 * Parses a Golay-encoded datagram header.
 */
enum lfc_res
lfc_dg_hdr__des(struct lfc_dg_hdr * out, struct cursor * csr);


struct lfc_dg_monolithic_flags
lfc_dg_monolithic_flags__des(uint8_t flag_bits);


struct lfc_dg_frame_start_flags
lfc_dg_frame_start_flags__des(uint8_t flag_bits);


struct lfc_dg_frame_data_flags
lfc_dg_frame_data_flags__des(uint8_t flag_bits);


struct lfc_dg_ack_flags
lfc_dg_ack_flags__des(uint8_t flag_bits);


#ifdef __cplusplus
}
#endif

/** @} */

#endif /* LFC_DG_DES_25E7091F */
