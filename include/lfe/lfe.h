#ifndef LFE_0ED159C6
#define LFE_0ED159C6

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
enum lfe_region {
    /** USA */
    lfe_region_fcc,
};

/**
 * LongFi engine user configuration.
 */
struct lfe_user_cfg {};

/**
 * LongFi engine context.
 *
 * More
 */
struct lfe {
    /** User-provided configuration. */
    struct lfe_user_cfg cfg;
};

/**
 * LoRa spreading factor.
 *
 * Low spreading factors spend less time on air but are more
 * susceptible to errors, where as large spreading factors are more
 * tolerant to noise but are slow to transmit.
 */
enum lfe_sf {
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
 * Initializes a user-provided `lfe` object.
 */
void
lfe_init(struct lfe const * lfe);

/**
 * Push a received payload into the engine.
 */
void
lfe_push_rx(struct lfe const * lfe, uint8_t const * pay, size_t pay_len);

/**
 * Initializes and returns a transmit plan.
 */
void
lfe_transmit(struct lfe const * lfe, uint8_t const * pay, size_t pay_len);


#ifdef __cplusplus
}
#endif

#endif /* LFE_0ED159C6 */
