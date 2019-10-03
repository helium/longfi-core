#include <assert.h>
#include <cursor/varint.h>
#include <stdbool.h>

static inline bool
_is_extended(uint8_t b) {
    return !!(b & 128);
}

enum cursor_res
cursor_unpack_var_le_u32(struct cursor * csr, uint32_t * dst) {
    uint32_t        out = 0;
    enum cursor_res res = cursor_res_ok;

    for (size_t i = 0; i <= sizeof(*dst); i++) {
        uint8_t input;
        res = cursor_unpack_le_u8(csr, &input);
        if (res != cursor_res_ok) {
            return res;
        }
        out |= (input & 127) << (7 * i);
        if (!_is_extended(input)) {
            break;
        }
    }

    *dst = out;
    return res;
}

enum cursor_res
cursor_pack_var_le_u32(struct cursor * csr, uint32_t val) {
    enum cursor_res res = cursor_res_ok;
    while (val > 127) {
        /* Write 7 bits of data to `csr` and set most significant bit. */
        res = cursor_pack_le_u8(csr, (val & 127) | 128);
        if (res != cursor_res_ok) {
            return res;
        }
        val >>= 7;
    }
    /* Write the remaining 7 bits to `csr`. */
    return cursor_pack_le_u8(csr, (val & 127));
}
