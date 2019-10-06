#ifndef DATAGRAM_3E0A3D4A
#define DATAGRAM_3E0A3D4A

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @file
 *
 * Datagram definitions.
 *
 * @addtogroup Datagram
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif


/** Constants */
enum CONSTANTS {
    /** Max supported datagram payload length. */
    CONSTANTS_MAX_PAY_LEN = 128,
};


/**
 * The type (or variant) of datagram.
 */
enum lfe_dg_type {
    /** Monolithic. */
    lfe_dg_type_monolithic = 1,
    /** Start of Frame. */
    lfe_dg_type_frame_start = 2,
    /** Frame Data. */
    lfe_dg_type_frame_data = 3,
    /** Ack. */
    lfe_dg_type_ack = 4,
};


/**
 * The result of parsing the golay-encoded header.
 */
struct lfe_dg_hdr {
    /** Number of corrected bit errors. */
    int bit_errs;
    /** Unparsed flags bits. */
    uint8_t flag_bits;
    /** This datagram's type */
    enum lfe_dg_type type;
};


/**
 * Monolithic.
 *
 * A Monolithic Datagram is expected to be the most common way to
 * transmit data that can fit within a single Datagram. When sending
 * or receiving small amounts of data, a Monolithic Datagram should be
 * used.
 */
struct lfe_dg_monolithic {
    /** Datagram flags. */
    struct lfe_dg_monolithic_flags {
        /**
         * This packet is destined for a Device if this bit is set.
         */
        bool downlink;
        /**
         * The receiver of this packet should acknowledge receipt.
         */
        bool should_ack;
        /**
         * On uplink this bit indicates the device is ready to
         * receive, on downlink it indicates further information
         * follows.
         */
        bool cts_rts;
        /**
         * This indicates to the receiver that the packet is deemed
         * urgent by the sender and the receiver can choose to act
         * accordingly.
         */
        bool priority;
        /**
         * The packet, beyond the Tag field, is encoded with a Low
         * Density Parity Code. The specific code used depends on the
         * maximum datagram size for the current region and spreading
         * factor.
         */
        bool ldpc;
    } flags;
    /** Organization ID. */
    uint32_t oui;
    /** Device ID. */
    uint32_t did;
    /** Fingerprint. */
    uint32_t fp;
    /** Sequence number. */
    uint32_t seq;
    /** Length (not capacity) of following payload. */
    size_t pay_len;
    /** Data payload. */
    uint8_t pay[CONSTANTS_MAX_PAY_LEN];
};


/**
 * Start of Frame.
 *
 * A Start of Frame Datagram is used to describe a series of following
 * Datagrams that contain a payload larger than a Monolithic Datagram
 * can hold. In some cases, if there's room, some of the payload may
 * appear at the end of this Datagram.
 */
struct lfe_dg_frame_start {
    /** Datagram flags. */
    struct lfe_dg_frame_start_flags {
        /**
         * This packet is destined for a Device if this bit is set.
         */
        bool downlink;
        /**
         * The receiver of this packet should acknowledge receipt.
         */
        bool should_ack;
        /**
         * On uplink this bit indicates the device is ready to
         * receive, on downlink it indicates further information
         * follows.
         */
        bool cts_rts;
        /**
         * This indicates to the receiver that the packet is deemed
         * urgent by the sender and the receiver can choose to act
         * accordingly.
         */
        bool priority;
        /**
         * The packet, beyond the Tag field, is encoded with a Low
         * Density Parity Code. The specific code used depends on the
         * maximum datagram size for the current region and spreading
         * factor.
         */
        bool ldpc;
    } flags;
    /** Organization ID. */
    uint32_t oui;
    /** Device ID. */
    uint32_t did;
    /** Fingerprint. */
    uint32_t fp;
    /** Fragment count. */
    uint32_t fragments;
    /** Length (not capacity) of following payload. */
    size_t pay_len;
    /** Data payload. */
    uint8_t pay[CONSTANTS_MAX_PAY_LEN];
};


/**
 * Frame Data.
 *
 * The Frame Data Datagram contains chunks of payload data that
 * correspond to a previous Start of Frame Datagram. The Sequence ID
 * should be included in the fingerprint generation to avoid crosstalk
 * between frames, but it is not needed to transmit it. The fragment
 * number is used to determine the ordering of the payload fragments.
 */
struct lfe_dg_frame_data {};


/**
 * Ack.
 *
 * The Ack datagram is used to indicate success/failure of a prior
 * transmission, in response to the prior transmission having the
 * Should ACK flag set. The Ack datagram can indicate success or
 * failure and request retransmits.
 */
struct lfe_dg_ack {
    /**
     * Ack-specific flags.
     */
    struct lfe_dg_ack_flags {
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
    } flags;
};


#ifdef __cplusplus
}
#endif

/** @} */

#endif /* DATAGRAM_3E0A3D4A */
