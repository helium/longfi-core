#include "lfe_dg_parse.h"
#include "golay/golay.h"
#include <assert.h>
#include <stdbool.h>

enum _lfe_dg_parse_res
_lfe_dg_parse(uint8_t const * pay, size_t pay_len, struct _lfe_dg_parsed * parsed) {
    return _lfe_dg_parse_res_ok;
}

struct _lfe_dg_flags
_lfe_dg_parse_flags(uint8_t flag_bits) {
    return (struct _lfe_dg_flags){
        .failure         = ((flag_bits >> 0) & 1) == 1,
        .session_expired = ((flag_bits >> 1) & 1) == 1,
        .cts_rts         = ((flag_bits >> 2) & 1) == 1,
        .retransmit      = ((flag_bits >> 3) & 1) == 1,
        .ldpc            = ((flag_bits >> 4) & 1) == 1,
        .reserved        = ((flag_bits >> 5) & 1) == 1,
    };
}

enum _lfe_dg_parse_res
_lfe_dg_parse_type(uint8_t type_bits, enum _lfe_dg_type * parsed) {
    if (type_bits == 0 || type_bits > _lfe_dg_type_ack) {
        return _lfe_dg_parse_res_invalid_type;
    }
    *parsed = (enum _lfe_dg_type)type_bits;
    return _lfe_dg_parse_res_ok;
}


enum _lfe_dg_parse_res
_lfe_dg_parse_header(uint8_t const * enc_hdr_bits, struct _lfe_dg_hdr * parsed) {
    enum _lfe_dg_parse_res res;
    uint16_t               dec_hdr_bits;
    uint8_t                dec_bit_errs =
        golay_decode(6, enc_hdr_bits, (uint8_t *)&dec_hdr_bits);

    bool    dg_extended_bit = 1 == ((dec_hdr_bits >> 12) & 1);
    uint8_t dg_type_bits    = (dec_hdr_bits >> 6 & 0x1F /* 0b1_1111 */);
    uint8_t dg_flags_bits   = (dec_hdr_bits & 0x3F /* 0b11_1111 */);
    assert(!dg_extended_bit);

    *parsed = (struct _lfe_dg_hdr){
        .bit_errs = dec_bit_errs,
        .flags    = _lfe_dg_parse_flags(dg_flags_bits),
    };

    if ((res = _lfe_dg_parse_type(dg_type_bits, &parsed->type))) {
        return res;
    }

    return _lfe_dg_parse_res_ok;
}
