#include "greatest/greatest.h"
#include "lfc/lfc.h"
#include "lfc/priv/lfc_dg_ser.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

enum { SESSION_KEY_MAX_LEN = 32 };

FILE * frnd = NULL;

uint32_t
ru32() {
    uint32_t out;
    fread(&out, 1, sizeof(out), frnd);
    return out;
}

size_t
gen_payload(uint8_t payload[static LFC_DG_CONSTANTS_MAX_PAY_LEN]) {
    memset(payload, 0, LFC_DG_CONSTANTS_MAX_PAY_LEN);
    size_t len = ru32() % (LFC_DG_CONSTANTS_MAX_PAY_LEN + 1);
    fread(payload, 1, len, frnd);
    return len;
}

size_t
gen_key(uint8_t key[static SESSION_KEY_MAX_LEN]) {
    size_t key_len = ru32() % (SESSION_KEY_MAX_LEN + 1);
    assert(key_len <= SESSION_KEY_MAX_LEN);
    fread(key, 1, key_len, frnd);
    return key_len;
}

struct lfc
gen_lfc() {
    /* `session_key` is `static` so it outlives `cfg`'s pointer to it. */
    static uint8_t      session_key[SESSION_KEY_MAX_LEN];
    size_t              session_key_len = gen_key(session_key);
    struct lfc_user_cfg cfg             = (struct lfc_user_cfg){
        .oui     = ru32(),
        .did     = ru32(),
        .key     = session_key,
        .key_len = session_key_len,
    };
    struct lfc lfc;
    lfc_init(&lfc, cfg);
    return lfc;
}

TEST
test_tx_rx_passes() {
    uint8_t payload[LFC_DG_CONSTANTS_MAX_PAY_LEN];
    size_t  payload_len = gen_payload(payload);

    uint8_t encoded[256];
    size_t  encoded_len = sizeof(encoded);

    struct lfc lfc = gen_lfc();

    ASSERT_EQ_FMT(lfc_res_ok,
                  lfc_transmit(&lfc, payload, payload_len, encoded, &encoded_len),
                  "%d");

    uint8_t decoded[256];
    size_t  decoded_len = sizeof(decoded);
    ASSERT_EQ_FMT(lfc_res_ok,
                  lfc_receive(&lfc, encoded, encoded_len, decoded, &decoded_len),
                  "%d");
    ASSERT_EQ_FMT(decoded_len, payload_len, "%zu");
    ASSERT_MEM_EQ(decoded, payload, decoded_len);

    GREATEST_PASS();
}

GREATEST_MAIN_DEFS();

int
main(int argc, char * argv[]) {
    GREATEST_MAIN_BEGIN();
    frnd = fopen("/dev/urandom", "r");
    assert(frnd);
    for (int i = 0; i < 100; ++i) {
        GREATEST_RUN_TEST(test_tx_rx_passes);
    }
    fclose(frnd);
    GREATEST_MAIN_END();
}
