#undef NDEBUG

#include <assert.h>
#include <cursor/cursor.h>
#include <stdint.h>
#include <stdlib.h>

int
main(int argc, char * argv[]) {
    (void)argc;
    (void)argv;

    /* Create a buffer to write packed primitives into */
    uint8_t buf[42];

    /* Construct a new cursor with the buffer we just defined */
    struct cursor csr = cursor_new(buf, sizeof(buf));

    /* Primitives we'll pack into the buffer */
    uint8_t  packed_u8  = rand();
    int8_t   packed_i8  = rand();
    uint16_t packed_u16 = rand();
    int16_t  packed_i16 = rand();
    uint32_t packed_u32 = rand();
    int32_t  packed_i32 = rand();
    uint64_t packed_u64 = rand();
    int64_t  packed_i64 = rand();
    float    packed_f   = 8.8;
    double   packed_d   = 9.9;

    /* A mixture of little/big endian packing
     * Note that same function call is used for all primitive types.
     * `cursor_pack_be` is actually macro using c11's generic selector feature.
     * It dispatches to the correct cursor function call based on the primitive
     * type */
    assert(cursor_pack_be(&csr, packed_u8) == cursor_res_ok);
    assert(cursor_pack_le(&csr, packed_i8) == cursor_res_ok);
    assert(cursor_pack_be(&csr, packed_u16) == cursor_res_ok);
    assert(cursor_pack_le(&csr, packed_i16) == cursor_res_ok);
    assert(cursor_pack_be(&csr, packed_u32) == cursor_res_ok);
    assert(cursor_pack_le(&csr, packed_i32) == cursor_res_ok);
    assert(cursor_pack_be(&csr, packed_u64) == cursor_res_ok);
    assert(cursor_pack_le(&csr, packed_i64) == cursor_res_ok);
    assert(cursor_pack_be(&csr, packed_f) == cursor_res_ok);
    assert(cursor_pack_le(&csr, packed_d) == cursor_res_ok);

    /* Our buffer was sized to have no leftover space.
     * Let's verify that attempting another pack operation returns an error.
     * Note that we're explicitly calling `cursor_pack_le_u8` here, and not
     * using the type-generic `cursor_pack_le` macro */
    assert(cursor_pack_le_u16(&csr, rand()) == cursor_res_err_buf_exhausted);

    /* We'll unpack our packed variables into these variables */
    uint8_t  unpacked_u8;
    int8_t   unpacked_i8;
    uint16_t unpacked_u16;
    int16_t  unpacked_i16;
    uint32_t unpacked_u32;
    int32_t  unpacked_i32;
    uint64_t unpacked_u64;
    int64_t  unpacked_i64;
    float    unpacked_f;
    double   unpacked_d;

    /* Reset the cursor.
     * `cursor_new` does not allocate so this is safe */
    csr = cursor_new(buf, sizeof(buf));

    /* Now unpack out of the buffer we packed into earlier  */
    assert(cursor_unpack_be(&csr, &unpacked_u8) == cursor_res_ok);
    assert(cursor_unpack_le(&csr, &unpacked_i8) == cursor_res_ok);
    assert(cursor_unpack_be(&csr, &unpacked_u16) == cursor_res_ok);
    assert(cursor_unpack_le(&csr, &unpacked_i16) == cursor_res_ok);
    assert(cursor_unpack_be(&csr, &unpacked_u32) == cursor_res_ok);
    assert(cursor_unpack_le(&csr, &unpacked_i32) == cursor_res_ok);
    assert(cursor_unpack_be(&csr, &unpacked_u64) == cursor_res_ok);
    assert(cursor_unpack_le(&csr, &unpacked_i64) == cursor_res_ok);
    assert(cursor_unpack_be(&csr, &unpacked_f) == cursor_res_ok);
    assert(cursor_unpack_le(&csr, &unpacked_d) == cursor_res_ok);

    /* Check the packed and unpacked variables for equality */
    assert(packed_u8 == unpacked_u8);
    assert(packed_i8 == unpacked_i8);
    assert(packed_u16 == unpacked_u16);
    assert(packed_i16 == unpacked_i16);
    assert(packed_u32 == unpacked_u32);
    assert(packed_i32 == unpacked_i32);
    assert(packed_u64 == unpacked_u64);
    assert(packed_i64 == unpacked_i64);
    assert(packed_f == unpacked_f);
    assert(packed_d == unpacked_d);

    return 0;
}
