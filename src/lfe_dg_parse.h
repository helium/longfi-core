#ifndef LFE_RX_PARSE_3067E11F
#define LFE_RX_PARSE_3067E11F

#include <lfe/lfe.h>

/**
 * Returned by parser to represent success or failure.
 */
enum _lfe_dg_parse_err {
    _lfe_dg_parse_err_ok,
    _lfe_dg_parse_err_invalid,
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
int
_lfe_dg_parse(struct lfe * lfe, uint8_t const * pay, size_t pay_len);


#endif /* LFE_RX_PARSE_3067E11F */
