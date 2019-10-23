#ifndef LFC_314C7FBF
#define LFC_314C7FBF

/**
 * @file
 */

#include "datagram.h"
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Return codes for top-level API calls.
 */
enum lfc_res {
    /** Success, no error. */
    lfc_res_ok,
    /** Received datagram does not match configured OUI or DID. */
    lfc_res_err_addr,
    /** Received datagram's fingerprint does not match locally
     * computed fingerprint. */
    lfc_res_err_fingerprint,
    /** Generic, exceptional error. */
    lfc_res_err_exception,
    /** Provided buffer is too small for request. */
    lfc_res_err_nomem,
    /** Invalid datagram type. */
    lfc_res_invalid_type,
    /** Invalid datagram flags. */
    lfc_res_invalid_flags,
};

/**
 * LongFi user configuration.
 */
struct lfc_user_cfg {
    /** Data provided by user, and included with every callback. */
    void * cb_data;
    /** Organizational Unique Identifier. */
    uint32_t oui;
    /** Device ID. */
    uint32_t did;
    /** Session key. */
    void const * key;
    /** Size (in bytes) of session key. */
    size_t key_len;
};

/**
 * LongFi context.
 */
struct lfc {
    /** Monotonically increasing number included in datagrams. */
    uint32_t seq;
    /** User-provided configuration. */
    struct lfc_user_cfg cfg;
};

/**
 * Initializes a user-provided `lfc` object.
 */
void
lfc_init(struct lfc * lfc, struct lfc_user_cfg cfg);

/**
 * Decodes a datagram from `in` buffer.
 *
 * @param lfc               LongFi Context.
 * @param in[in]            Buffer containing an encoded datagram.
 * @param in_len            Length of `in`.
 * @param out[out]          Buffer to write decoded payload to.
 * @param[in,out] dg_len    in: capacity of `out` buffer.
 *                          out: actual size of payload.
 */
enum lfc_res
lfc_receive(struct lfc const * lfc,
            uint8_t const *    in,
            size_t             in_len,
            uint8_t *          out,
            size_t *           out_len);

/**
 * Creates and encodes a datagram into the provided `out` buffer from
 * `pay`.
 *
 * @param lfc               LongFi Context.
 * @param pay               Payload you want to send.
 * @param pay_len           Length of `pay`.
 * @param out               Buffer to serialize datagram into.
 * @param[in,out] dg_len    in: capacity of `out` buffer.
 *                          out: actual serialized size of datagram.
 */
enum lfc_res
lfc_transmit(struct lfc *    lfc,
             uint8_t const * pay,
             size_t          pay_len,
             uint8_t *       out,
             size_t *        out_len);


#ifdef __cplusplus
}
#endif

#endif /* LFC_314C7FBF */
