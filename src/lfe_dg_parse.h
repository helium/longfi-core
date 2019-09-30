#ifndef LFE_RX_PARSE_3067E11F
#define LFE_RX_PARSE_3067E11F

/**
 * @file
 *
 * Private API for parsing received datagrams.
 *
 * @addtogroup Private
 */

#include <lfe/lfe.h>

/**
 * Returned by parser to represent success or failure.
 */
enum _lfe_dg_parse_err {
    /** Operation successful. */
    _lfe_dg_parse_err_ok,
    /** Invalid data encountered while parsing datagram. */
    _lfe_dg_parse_err_invalid,
};


/**
 * The type (or variant) of datagram.
 */
enum _lfe_dg_type {
    /** Monolithic. A Monolithic Datagram is expected to be the most
     * common way to transmit data that can fit within a single
     * Datagram. When sending or receiving small amounts of data, a
     * Monolithic Datagram should be used.
     */
    _lfe_dg_type_monolithic,
    /** Start of Frame. A Start of Frame Datagram is used to describe
     * a series of following Datagrams that contain a payload larger
     * than a Monolithic Datagram can hold. In some cases, if there's
     * room, some of the payload may appear at the end of this
     * Datagram.
     */
    _lfe_dg_type_sof,
};

/**
 * The result of parsing the golay-encoded header.
 */
struct _lfe_dg_hdr {
    enum _lfe_dg_type type;
    uint8_t           flags;
};

/**
 * A successfully parsed datagram..
 */
struct _lfe_dg_parsed {
    /** Number of corrected bit errors in the header. */
    size_t hdr_bit_errs;
};

/**
 * Parses a received packet.
 */
enum _lfe_dg_parse_err
_lfe_dg_parse(struct lfe const * lfe, uint8_t const * pay, size_t pay_len);

/** @} */

#endif /* LFE_RX_PARSE_3067E11F */
