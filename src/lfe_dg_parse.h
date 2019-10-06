#ifndef LFE_DG_PARSE_72B18C92
#define LFE_DG_PARSE_72B18C92

#include "cursor/cursor.h"
#include "lfe/datagram.h"
#include "lfe/lfe.h"
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
 * Returned by parser to represent success or failure.
 */
enum lfe_dg_parse_res {
    /** Operation successful. */
    lfe_dg_parse_res_ok,
    /** Reached end of provided data before complete parse. */
    lfe_dg_parse_res_err_nomem,
    /** Invalid datagram type. */
    lfe_dg_parse_res_invalid_type,
    /** Invalid datagram flags. */
    lfe_dg_parse_res_invalid_flags,
};


/**
 * A successfully parsed datagram.
 */
struct lfe_dg_parsed {
    /**
     * Number of corrected bit errors in the header.
     */
    size_t hdr_bit_errs;
    /**
     * This datagram's type.
     *
     * Indicates which member of `u` is valid.
     */
    enum lfe_dg_type type;
    union {
        struct lfe_dg_monolithic  monolithic;
        struct lfe_dg_frame_start frame_start;
        struct lfe_dg_frame_data  frame_data;
        struct lfe_dg_ack         ack;
    };
};


/**
 * Parses a received packet.
 */
enum lfe_dg_parse_res
lfe_dg__parse(struct lfe_dg_parsed * out, struct cursor * csr);


/**
 * Parses a `monolithic` datagram.
 */
enum lfe_dg_parse_res
lfe_dg_monolithic__parse(struct lfe_dg_monolithic * out,
                         struct lfe_dg_hdr          hdr,
                         struct cursor *            csr);


/**
 * Parses a `ack` datagram.
 */
enum lfe_dg_parse_res
lfe_dg_ack__parse(struct lfe_dg_ack * out,
                  struct lfe_dg_hdr   hdr,
                  struct cursor *     csr);


/**
 * Parses a `frame_start` datagram.
 */
enum lfe_dg_parse_res
lfe_dg_frame_start__parse(struct lfe_dg_frame_start * out,
                          struct lfe_dg_hdr           hdr,
                          struct cursor *             csr);


/**
 * Parses a `frame_data` datagram.
 */
enum lfe_dg_parse_res
lfe_dg_frame_data__parse(struct lfe_dg_frame_data * out,
                         struct lfe_dg_hdr          hdr,
                         struct cursor *            csr);


/**
 * Parses a Golay-encoded datagram header.
 */
enum lfe_dg_parse_res
lfe_dg_hdr__parse(struct lfe_dg_hdr * out, struct cursor * csr);


struct lfe_dg_monolithic_flags
lfe_dg_monolithic_flags__parse(uint8_t flag_bits);


struct lfe_dg_frame_start_flags
lfe_dg_frame_start_flags__parse(uint8_t flag_bits);


struct lfe_dg_frame_data_flags
lfe_dg_frame_data_flags__parse(uint8_t flag_bits);


struct lfe_dg_ack_flags
lfe_dg_ack_flags__parse(uint8_t flag_bits);


#ifdef __cplusplus
}
#endif

/** @} */

#endif /* LFE_DG_PARSE_72B18C92 */
