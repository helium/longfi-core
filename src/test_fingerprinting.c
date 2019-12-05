#include "greatest/greatest.h"
#include "lfc/fingerprint.h"
#include "lfc/lfc.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

enum { SESSION_KEY_MAX_LEN = 32 };

FILE * frnd = NULL;

uint32_t
ru32() {
    uint32_t out;
    int      n = fread(&out, sizeof(out), 1, frnd);
    assert(1 == n);
    (void)n;
    return out;
}

size_t
gen_payload(uint8_t payload[static LFC_DG_CONSTANTS_MAX_PAY_LEN]) {
    memset(payload, 0, LFC_DG_CONSTANTS_MAX_PAY_LEN);
    size_t len = ru32() % (LFC_DG_CONSTANTS_MAX_PAY_LEN + 1);
    size_t n   = fread(payload, 1, len, frnd);
    assert(len == n);
    (void)n;
    return len;
}

size_t
gen_key(uint8_t key[static SESSION_KEY_MAX_LEN]) {
    size_t key_len = ru32() % (SESSION_KEY_MAX_LEN + 1);
    assert(key_len <= SESSION_KEY_MAX_LEN);
    size_t n = fread(key, 1, key_len, frnd);
    assert(key_len == n);
    (void)n;
    return key_len;
}

struct test_node_params {
    size_t   key_len;
    uint8_t  key[SESSION_KEY_MAX_LEN];
    uint16_t hdr;
    uint32_t oui;
    uint32_t did;
    uint32_t seq;
    size_t   pay_len;
    uint8_t  pay[LFC_DG_CONSTANTS_MAX_PAY_LEN];
    uint8_t  pay_hash[32];
};

struct test_node_params
gen_test_node_params() {
    struct test_node_params plan = {
        .key_len = gen_key(plan.key),
        .hdr     = ru32() & 0xFFF,
        .oui     = ru32(),
        .did     = ru32(),
        .seq     = ru32(),
        .pay_len = gen_payload(plan.pay),
    };
    enum lfc_res res = lfc_payload_hash(plan.pay,
                                        plan.pay_len,
                                        plan.pay_hash,
                                        sizeof(plan.pay_hash));
    assert(res == lfc_res_ok);
    (void)res;
    return plan;
}

enum test_corruption {
    test_corruption_none,
    test_corruption_key,
    test_corruption_hdr,
    test_corruption_oui,
    test_corruption_did,
    test_corruption_seq,
    test_corruption_pay,
};

const char *
test_corruption_str(int value) {
    const char * ret;
    switch (value) {
    case test_corruption_none:
        ret = "test_corruption_none";
        break;
    case test_corruption_key:
        ret = "test_corruption_key";
        break;
    case test_corruption_hdr:
        ret = "test_corruption_hdr";
        break;
    case test_corruption_oui:
        ret = "test_corruption_oui";
        break;
    case test_corruption_did:
        ret = "test_corruption_did";
        break;
    case test_corruption_seq:
        ret = "test_corruption_seq";
        break;
    case test_corruption_pay:
        ret = "test_corruption_pay";
        break;
    default:
        ret = "";
        assert(0);
        break;
    }
    return ret;
}

enum test_corruption
gen_test_corruption() {
    int value = ru32() % (test_corruption_pay);
    assert(value >= test_corruption_none);
    assert(value <= test_corruption_pay);
    return (enum test_corruption)value;
}

void
test_node_params_corrupt(struct test_node_params * p, enum test_corruption c) {
    switch (c) {
    case test_corruption_none:
        break;
    case test_corruption_key: {
        uint8_t key[SESSION_KEY_MAX_LEN];
        size_t  key_len;
        do {
            key_len = gen_key(key);
        } while ((key_len == p->key_len) && (!memcmp(key, p->key, key_len)));
        memmove(p->key, key, key_len);
        p->key_len = key_len;
    } break;
    case test_corruption_hdr:
        p->hdr = (p->hdr + 1) & 0xFFF;
        break;
    case test_corruption_oui:
        p->oui += 1;
        break;
    case test_corruption_did:
        p->did += 1;
        break;
    case test_corruption_seq:
        p->seq += 1;
        break;
    case test_corruption_pay: {
        uint8_t pay[LFC_DG_CONSTANTS_MAX_PAY_LEN];
        size_t  pay_len;
        do {
            pay_len = gen_payload(pay);
        } while ((pay_len == p->pay_len) && (!memcmp(pay, p->pay, pay_len)));
        memmove(p->pay, pay, pay_len);
        p->pay_len = pay_len;
        enum lfc_res res =
            lfc_payload_hash(p->pay, p->pay_len, p->pay_hash, sizeof(p->pay_hash));
        assert(res == lfc_res_ok);
        (void)res;
    } break;
    }
}

struct test_plan {
    struct test_node_params device_node;
    enum test_corruption    corruption;
    struct test_node_params router_node;
};

struct test_plan
gen_test_plan() {
    struct test_plan plan;
    plan.device_node = gen_test_node_params();
    memmove(&plan.router_node, &plan.device_node, sizeof(plan.router_node));
    plan.corruption = gen_test_corruption();
    test_node_params_corrupt(&plan.router_node, plan.corruption);
    return plan;
}

TEST
test_fingerprinting_passes() {
    struct test_plan plan = gen_test_plan();
    if (plan.corruption == test_corruption_none) {
        ASSERT_MEM_EQ(&plan.device_node,
                      &plan.router_node,
                      sizeof(plan.device_node));
    }
    /* Emulate device-side. */
    uint32_t     fp  = UINT32_MAX - 1;
    enum lfc_res res = lfc_fingerprint_monolithic(plan.device_node.key,
                                                  plan.device_node.key_len,
                                                  plan.device_node.hdr,
                                                  plan.device_node.oui,
                                                  plan.device_node.did,
                                                  plan.device_node.seq,
                                                  plan.device_node.pay,
                                                  plan.device_node.pay_len,
                                                  &fp);
    ASSERT_EQ(lfc_res_ok, res);
    /* Emulate router-side. */
    res = lfc_fingerprint_monolithic_verify(fp,
                                            plan.router_node.key,
                                            plan.router_node.key_len,
                                            plan.router_node.pay_hash,
                                            sizeof(plan.router_node.pay_hash),
                                            plan.router_node.hdr,
                                            plan.router_node.oui,
                                            plan.router_node.did,
                                            plan.router_node.seq);

    if (res == lfc_res_ok) {
        GREATEST_ASSERT_ENUM_EQm("verfication returned ok, but",
                                 test_corruption_none,
                                 plan.corruption,
                                 test_corruption_str);
        PASS();
    } else if (plan.corruption == test_corruption_none) {
        FAIL();
    }
    PASS();
}

// cppcheck-suppress unreadVariable
GREATEST_MAIN_DEFS();

int
main(int argc, char * argv[]) {
    GREATEST_MAIN_BEGIN();
    frnd = fopen("/dev/urandom", "r");
    assert(frnd);
    for (int i = 0; i < 1000; ++i) {
        GREATEST_RUN_TEST(test_fingerprinting_passes);
    }
    fclose(frnd);
    GREATEST_MAIN_END();
}
