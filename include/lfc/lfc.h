#ifndef LFC_314C7FBF
#define LFC_314C7FBF

/**
 * @file
 */

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
 * LongFi regulatory region.
 *
 * LongFi users are responsible for configuring their devices' region
 * in order to remain compliant with regulations.
 */
enum lfc_region {
    /** USA */
    lfc_region_fcc,
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
 * LoRa spreading factor.
 *
 * Low spreading factors spend less time on air but are more
 * susceptible to errors, where as large spreading factors are more
 * tolerant to noise but are slow to transmit.
 */
enum lfc_sf {
    sf_5  = 5,
    sf_6  = 6,
    sf_7  = 7,
    sf_8  = 8,
    sf_9  = 9,
    sf_10 = 10,
    sf_11 = 11,
    sf_12 = 12,
};

/**
 * Initializes a user-provided `lfc` object.
 */
void
lfc_init(struct lfc * lfc, struct lfc_user_cfg cfg);

/**
 * Push a received payload into the context.
 */
void
lfc_push_rx(struct lfc const * lfc, uint8_t const * pay, size_t pay_len);

/**
 * Initializes and returns a transmit plan.
 *
 *
 * @param lfc               LongFi Context.
 * @param msg               Payload you want to send.
 * @param msg_len           Length of `pay`.
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
