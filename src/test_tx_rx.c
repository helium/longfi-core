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
    volatile size_t len = ru32() % (LFC_DG_CONSTANTS_MAX_PAY_LEN + 1);
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

TEST
test_tx_rx_roundtrips() {
    uint8_t session_key[SESSION_KEY_MAX_LEN];
    size_t  session_key_len = gen_key(session_key);

    uint8_t payload[LFC_DG_CONSTANTS_MAX_PAY_LEN];
    size_t  payload_len = gen_payload(payload);

    struct lfc_user_cfg cfg = (struct lfc_user_cfg){
        .oui     = ru32(),
        .did     = ru32(),
        .key     = session_key,
        .key_len = session_key_len,
    };

    struct lfc lfc;
    lfc_init(&lfc, cfg);

    uint8_t      serialized[256];
    size_t       serialized_len = sizeof(serialized);
    enum lfc_res res =
        lfc_transmit(&lfc, payload, payload_len, serialized, &serialized_len);
    ASSERT_EQ(lfc_res_ok, res);

    GREATEST_PASS();
}


GREATEST_MAIN_DEFS();

int
main(int argc, char * argv[]) {
    GREATEST_MAIN_BEGIN();
    frnd = fopen("/dev/urandom", "r");
    assert(frnd);
    for (int i = 0; i < 100; ++i) {
        GREATEST_RUN_TEST(test_tx_rx_roundtrips);
    }
    fclose(frnd);
    GREATEST_MAIN_END();
}
