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
#include <stdbool.h>

/** Constants */
enum constants {
    /** Max supported datagram payload length. */
    constants_max_pay_len = 128,
};

/**
 * Returned by parser to represent success or failure.
 */
enum _lfe_dg_parse_res {
    /** Operation successful. */
    _lfe_dg_parse_res_ok,
    /** Invalid datagram type. */
    _lfe_dg_parse_res_invalid_type,
    /** Invalid datagram flags. */
    _lfe_dg_parse_res_invalid_flags,
};


/**
 * The type (or variant) of datagram.
 */
enum _lfe_dg_type {
    /** Monolithic. */
    _lfe_dg_type_monolithic = 1,
    /** Start of Frame. */
    _lfe_dg_type_frame_start = 2,
    /** Frame Data. */
    _lfe_dg_type_frame_data = 3,
    /** Ack. */
    _lfe_dg_type_ack = 4,
};


/**
 * Represents bit flags in a datagram header.
 */
struct _lfe_dg_flags {
    /**
     * The receiver was unable to receive the previous message.
     */
    bool failure;
    /**
     * Downlink only.
     *
     * The receiver requests the sender to renegotiate a session key.
     */
    bool session_expired;
    /**
     * Clear-to-send/ready-to-send.
     *
     * On uplink this bit indicates the device is ready to receive, on
     * downlink it indicates further information follows.
     */
    bool cts_rts;
    /**
     * Retransmit requested.
     */
    bool retransmit;
    /**
     * LDCP Encoding.
     *
     * The packet, beyond the Tag field, is encoded with a [Low
     * Density Parity
     * Code](https://en.wikipedia.org/wiki/Low-density_parity-check_code).
     * The specific code used depends on the maximum datagram size for
     * the current region and spreading factor.
     */
    bool ldpc;
    /**
     * Reserved.
     */
    bool reserved;
};


/**
 * The result of parsing the golay-encoded header.
 */
struct _lfe_dg_hdr {
    /** Number of corrected bit errors. */
    size_t bit_errs;
    /** Parsed flags. */
    struct _lfe_dg_flags flags;
    /** This datagram's type */
    enum _lfe_dg_type type;
};


/**
 * Monolithic.
 *
 * A Monolithic Datagram is expected to be the most common way to
 * transmit data that can fit within a single Datagram. When sending
 * or receiving small amounts of data, a Monolithic Datagram should be
 * used.
 */
struct _lfe_dg_monolithic {
    /** Datagram flags. */
    struct _lfe_dg_flags flags;
    /** Organization ID. */
    uint32_t oui;
    /** Device ID. */
    uint32_t did;
    /** Fingerprint. */
    uint32_t fp;
    /** Sequence number. */
    uint32_t seq;
    /** Data payload. */
    uint8_t pay[constants_max_pay_len];
};

/**
 * Start of Frame.
 *
 * A Start of Frame Datagram is used to describe a series of following
 * Datagrams that contain a payload larger than a Monolithic Datagram
 * can hold. In some cases, if there's room, some of the payload may
 * appear at the end of this Datagram.
 */
struct _lfe_dg_frame_start {};


/**
 * Frame Data.
 *
 * The Frame Data Datagram contains chunks of payload data that
 * correspond to a previous Start of Frame Datagram. The Sequence ID
 * should be included in the fingerprint generation to avoid crosstalk
 * between frames, but it is not needed to transmit it. The fragment
 * number is used to determine the ordering of the payload fragments.
 */
struct _lfe_dg_frame_data {};


/**
 * Ack.
 *
 * The Ack datagram is used to indicate success/failure of a prior
 * transmission, in response to the prior transmission having the
 * Should ACK flag set. The Ack datagram can indicate success or
 * failure and request retransmits.
 */
struct _lfe_dg_ack {};


/**
 * A successfully parsed datagram.
 */
struct _lfe_dg_parsed {
    /**
     * Number of corrected bit errors in the header.
     */
    size_t hdr_bit_errs;
    /**
     * This datagram's type.
     *
     * Indicates which member of `u` is valid.
     */
    enum _lfe_dg_type type;
    union {
        struct _lfe_dg_monolithic  monolithic;
        struct _lfe_dg_frame_start frame_start;
        struct _lfe_dg_frame_data  frame_data;
        struct _lfe_dg_ack         ack;
    };
};


/**
 * Parses a received packet.
 */
enum _lfe_dg_parse_res
_lfe_dg_parse(uint8_t const * pay, size_t pay_len, struct _lfe_dg_parsed * parsed);


/**
 * Parses a Golay-encoded datagram header.
 */
enum _lfe_dg_parse_res
_lfe_dg_parse_header(uint8_t const * enc_hdr_bits, struct _lfe_dg_hdr * parsed);


/** @} */

#endif /* LFE_RX_PARSE_3067E11F */
