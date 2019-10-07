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
struct lfc_user_cfg {};

/**
 * LongFi context.
 */
struct lfc {
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
lfc_init(struct lfc const * lfc);

/**
 * Push a received payload into the context.
 */
void
lfc_push_rx(struct lfc const * lfc, uint8_t const * pay, size_t pay_len);

/**
 * Initializes and returns a transmit plan.
 */
void
lfc_transmit(struct lfc const * lfc, uint8_t const * pay, size_t pay_len);


#ifdef __cplusplus
}
#endif

#endif /* LFC_314C7FBF */
