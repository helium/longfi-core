#include "lfe_dg_parse.h"
#include <assert.h>
#include <golay/golay.h>

enum _lfe_dg_parse_err
_lfe_dg_parse(struct lfe const * lfe, uint8_t const * pay, size_t pay_len) {
    return _lfe_dg_parse_err_invalid;
}

enum _lfe_dg_parse_err
_lfe_dg_parse_header(struct lfe const * lfe, uint8_t const * enc_hdr_bits) {
    uint8_t dec_hdr_bits[3];
    uint8_t dec_bit_errs = golay_decode(6, enc_hdr_bits, dec_hdr_bits);

    return _lfe_dg_parse_err_ok;
}
