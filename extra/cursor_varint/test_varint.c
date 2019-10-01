#include <assert.h>
#include <cursor/varint.h>
#include <greatest/greatest.h>
#include <stdlib.h>

uint32_t
rand_u32() {
    return lrand48();
}

TEST
test_var_u32_roundtrips(uint32_t const val) {
    uint8_t         buf[5];
    struct cursor   csr = cursor_new(buf, sizeof(buf));
    enum cursor_res res = cursor_pack_var_le_u32(&csr, val);
    ASSERT_EQ(cursor_res_ok, res);
    uint32_t unpacked = UINT32_MAX;
    csr               = cursor_new(buf, sizeof(buf));
    cursor_unpack_var_le_u32(&csr, &unpacked);
    ASSERT_EQ_FMT(val, unpacked, "%08X");
    PASS();
}

GREATEST_MAIN_DEFS();

int
main(int argc, char * argv[]) {
    GREATEST_MAIN_BEGIN();
    for (int i = 0; i < 100; ++i) {
        RUN_TEST1(test_var_u32_roundtrips, rand_u32());
    }
    GREATEST_MAIN_END();
}
