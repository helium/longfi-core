#include <golay/golay.h>
#include <greatest/greatest.h>
#include <string.h>

struct golay_testcase {
    uint16_t val;
    uint32_t bitflips;
};

TEST
test_golay_12_roundtrips(struct golay_testcase tc) {
    uint8_t encoded[3];
    golay_encode12(tc.val, encoded);

    encoded[0] ^= tc.bitflips & 0xFF;
    encoded[1] ^= (tc.bitflips >> 8) & 0xFF;
    encoded[2] ^= (tc.bitflips >> 16) & 0xFF;

    uint16_t decoded;
    int8_t   bit_errs       = golay_decode12(encoded, &decoded);
    uint8_t  bitflip_errors = popcount_2(tc.bitflips & 0xFFFFFF);
    if (bitflip_errors <= 3) {
        // 100% of one- to three-bit errors corrected, any pattern
        ASSERT_EQ(bit_errs, bitflip_errors);
        ASSERT_EQ(tc.val, decoded);
        PASS();
    } else if (bitflip_errors == 4) {
        // 100% of four-bit errors detected, any pattern
        ASSERT_EQ(-1, bit_errs);
        PASS();
    } else if ((bitflip_errors % 2) == 1) {
        // 100% of odd numbers of bit errors detected, any pattern
        if (tc.val == decoded) {
            // sometimes it can pass
            ASSERT_EQ(bitflip_errors, bit_errs);
        } else {
            // this doesn't actually guarantee a decode failure?
            ASSERT(bit_errs != 0);
        }
        PASS();
    } else {
        // 0.24% of other errors corrected (1/4096)
        // unclear how to meaningfully test this
        PASS();
    }
}

GREATEST_MAIN_DEFS();

int
main(int argc, char * argv[]) {
    GREATEST_MAIN_BEGIN();

    struct golay_testcase tc;

    srand(time(NULL));

    for (uint16_t j = 0; j <= 500; ++j) {
        tc.bitflips = rand();
        /*tc.val = rand();*/
        /*RUN_TEST1(test_golay_12_roundtrips, tc);*/
        for (uint16_t i = 0; i <= 0xFFF; ++i) {
            tc.val = i;
            /*tc.bitflips = rand() & 0xFFFFFF;*/
            RUN_TEST1(test_golay_12_roundtrips, tc);
        }
    }

    GREATEST_MAIN_END();
}
