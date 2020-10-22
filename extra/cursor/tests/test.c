#include "greatest.h"
#include <assert.h>
#include <cursor/cursor.h>
#include <cursor/packing.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef HAVE_ATTRIBUTE_MAY_ALIAS
typedef uint64_t __attribute__((__may_alias__)) uint64_a;
#else
typedef uint64_t uint64_a;
#endif

static inline float
random_float() {
    return (float)random() / (float)(RAND_MAX);
}

static uint64_t
random_unique() {
    uint8_t initial = random();
    uint8_t val[sizeof(uint64_t)];
    for (size_t i = 0; i < sizeof(uint64_t); i++) {
        val[i] = initial++;
    }
    return *((uint64_a *)val);
}

enum instruction_type {
    it_u8,
    it_i8,
    it_u16,
    it_i16,
    it_u32,
    it_i32,
    it_u64,
    it_i64,
    it_f,
    it_d,
};
#define IT_VARIANT_CNT (it_d + 1)

/* A single 'instruction' in randmonized multi-part serialize/deserialize test
 * plan
 */
struct instruction {
    enum instruction_type it;
    union {
        uint8_t  u8;
        int8_t   i8;
        uint16_t u16;
        int16_t  i16;
        uint32_t u32;
        int32_t  i32;
        uint64_t u64;
        int64_t  i64;
        float    f;
        double   d;
    } val;
    enum {
        big,
        little,
    } endianness;
    size_t val_size;
};

struct instruction
random_instruction() {
    struct instruction inst;
    /* generate a random tag */
    int tag_idx = random_unique() % IT_VARIANT_CNT;
    switch (tag_idx) {
    case it_u8:
        inst.it       = it_u8;
        inst.val.u8   = random_unique();
        inst.val_size = sizeof(inst.val.u8);
        break;
    case it_i8:
        inst.it       = it_i8;
        inst.val.i8   = random_unique();
        inst.val_size = sizeof(inst.val.i8);
        break;
    case it_u16:
        inst.it       = it_u16;
        inst.val.u16  = random_unique();
        inst.val_size = sizeof(inst.val.u16);
        break;
    case it_i16:
        inst.it       = it_i16;
        inst.val.i16  = random_unique();
        inst.val_size = sizeof(inst.val.i16);
        break;
    case it_u32:
        inst.it       = it_u32;
        inst.val.u32  = random_unique();
        inst.val_size = sizeof(inst.val.u32);
        break;
    case it_i32:
        inst.it       = it_i32;
        inst.val.i32  = random_unique();
        inst.val_size = sizeof(inst.val.i32);
        break;
    case it_u64:
        inst.it       = it_u64;
        inst.val.u64  = random_unique();
        inst.val_size = sizeof(inst.val.u64);
        break;
    case it_i64:
        inst.it       = it_i64;
        inst.val.i64  = random_unique();
        inst.val_size = sizeof(inst.val.i64);
        break;
    case it_f:
        inst.it       = it_f;
        inst.val.f    = random_float();
        inst.val_size = sizeof(inst.val.f);
        break;
    case it_d:
        inst.it       = it_d;
        inst.val.d    = random_float();
        inst.val_size = sizeof(inst.val.d);
        break;
    default:
        fprintf(stderr, "Invalid instruction variant tag\n");
        assert(0);
    }
    /* Pick a random endianness */
    inst.endianness = rand() % 2;
    return inst;
}

struct plan {
    struct instruction * is;
    size_t               nis;
    size_t               req_buf_size;
};

struct plan
random_plan(struct instruction * is, size_t nis) {
    size_t req_buf_size = 0;
    for (size_t i = 0; i < nis; i++) {
        is[i] = random_instruction();
        req_buf_size += is[i].val_size;
    }

    struct plan plan = {
        .is           = is,
        .nis          = nis,
        .req_buf_size = req_buf_size,
    };
    return plan;
}

enum cursor_res
cursor_pack_instr_le(struct cursor * csr, struct instruction const * instr) {
    enum cursor_res res = cursor_res_ok;
    switch (instr->it) {
    case it_u8:
        res = cursor_pack_le(csr, instr->val.u8);
        break;
    case it_i8:
        res = cursor_pack_le(csr, instr->val.i8);
        break;
    case it_u16:
        res = cursor_pack_le(csr, instr->val.u16);
        break;
    case it_i16:
        res = cursor_pack_le(csr, instr->val.i16);
        break;
    case it_u32:
        res = cursor_pack_le(csr, instr->val.u32);
        break;
    case it_i32:
        res = cursor_pack_le(csr, instr->val.i32);
        break;
    case it_u64:
        res = cursor_pack_le(csr, instr->val.u64);
        break;
    case it_i64:
        res = cursor_pack_le(csr, instr->val.i64);
        break;
    case it_f:
        res = cursor_pack_le(csr, instr->val.f);
        break;
    case it_d:
        res = cursor_pack_le(csr, instr->val.d);
        break;
    }
    return res;
}

enum cursor_res
cursor_pack_instr_be(struct cursor * csr, struct instruction const * instr) {
    enum cursor_res res = cursor_res_ok;
    switch (instr->it) {
    case it_u8:
        res = cursor_pack_be(csr, instr->val.u8);
        break;
    case it_i8:
        res = cursor_pack_be(csr, instr->val.i8);
        break;
    case it_u16:
        res = cursor_pack_be(csr, instr->val.u16);
        break;
    case it_i16:
        res = cursor_pack_be(csr, instr->val.i16);
        break;
    case it_u32:
        res = cursor_pack_be(csr, instr->val.u32);
        break;
    case it_i32:
        res = cursor_pack_be(csr, instr->val.i32);
        break;
    case it_u64:
        res = cursor_pack_be(csr, instr->val.u64);
        break;
    case it_i64:
        res = cursor_pack_be(csr, instr->val.i64);
        break;
    case it_f:
        res = cursor_pack_be(csr, instr->val.f);
        break;
    case it_d:
        res = cursor_pack_be(csr, instr->val.d);
        break;
    }
    return res;
}

enum cursor_res
cursor_pack_instr(struct cursor * csr, struct instruction const * instr) {
    enum cursor_res res = cursor_res_ok;
    switch (instr->endianness) {
    case big:
        res = cursor_pack_instr_be(csr, instr);
        break;
    case little:
        res = cursor_pack_instr_le(csr, instr);
        break;
    }
    return res;
}

TEST
cursor_unpack_le_and_compare_to_instr(struct cursor *            csr,
                                      struct instruction const * instr) {
    char err_msg[64];
    snprintf(err_msg,
             sizeof(err_msg),
             "Unpack value does not match at position %zd",
             csr->pos);
    enum cursor_res res = cursor_res_ok;
    switch (instr->it) {
    case it_u8: {
        uint8_t val;
        res = cursor_unpack_le(csr, &val);
        ASSERT_EQ_FMTm(err_msg, instr->val.u8, val, "%" PRIu8);
    } break;
    case it_i8: {
        int8_t val;
        res = cursor_unpack_le(csr, &val);
        ASSERT_EQ_FMTm(err_msg, instr->val.i8, val, "%" PRId8);
        break;
    }
    case it_u16: {
        uint16_t val;
        res = cursor_unpack_le(csr, &val);
        ASSERT_EQ_FMTm(err_msg, instr->val.u16, val, "%" PRId16);
    } break;
    case it_i16: {
        int16_t val;
        res = cursor_unpack_le(csr, &val);
        ASSERT_EQ_FMTm(err_msg, instr->val.i16, val, "%" PRId16);
    } break;
    case it_u32: {
        uint32_t val;
        res = cursor_unpack_le(csr, &val);
        ASSERT_EQ_FMTm(err_msg, instr->val.u32, val, "%" PRIu32);
    } break;
    case it_i32: {
        int32_t val;
        res = cursor_unpack_le(csr, &val);
        ASSERT_EQ_FMT(instr->val.i32, val, "%" PRId32);
    } break;
    case it_u64: {
        uint64_t val;
        res = cursor_unpack_le(csr, &val);
        ASSERT_EQ_FMTm(err_msg, instr->val.u64, val, "%" PRIu64);
    } break;
    case it_i64: {
        int64_t val;
        res = cursor_unpack_le(csr, &val);
        ASSERT_EQ_FMTm(err_msg, instr->val.i64, val, "%" PRId64);
    } break;
    case it_f: {
        float val;
        res = cursor_unpack_le(csr, &val);
        ASSERT_EQ_FMTm(err_msg, instr->val.f, val, "%f");
    } break;
    case it_d: {
        double val;
        res = cursor_unpack_le(csr, &val);
        ASSERT_EQ_FMTm(err_msg, instr->val.d, val, "%f");
    } break;
    }
    ASSERT_EQ(cursor_res_ok, res);
    PASS();
}

TEST
cursor_unpack_be_and_compare_to_instr(struct cursor *            csr,
                                      struct instruction const * instr) {
    char err_msg[64];
    snprintf(err_msg,
             sizeof(err_msg),
             "Unpack value does not match at position %zd",
             csr->pos);
    enum cursor_res res = cursor_res_ok;
    switch (instr->it) {
    case it_u8: {
        uint8_t val;
        res = cursor_unpack_be(csr, &val);
        ASSERT_EQ_FMTm(err_msg, instr->val.u8, val, "%" PRIu8);
    } break;
    case it_i8: {
        int8_t val;
        res = cursor_unpack_be(csr, &val);
        ASSERT_EQ_FMTm(err_msg, instr->val.i8, val, "%" PRId8);
        break;
    }
    case it_u16: {
        uint16_t val;
        res = cursor_unpack_be(csr, &val);
        ASSERT_EQ_FMTm(err_msg, instr->val.u16, val, "%" PRId16);
    } break;
    case it_i16: {
        int16_t val;
        res = cursor_unpack_be(csr, &val);
        ASSERT_EQ_FMTm(err_msg, instr->val.i16, val, "%" PRId16);
    } break;
    case it_u32: {
        uint32_t val;
        res = cursor_unpack_be(csr, &val);
        ASSERT_EQ_FMTm(err_msg, instr->val.u32, val, "%" PRIu32);
    } break;
    case it_i32: {
        int32_t val;
        res = cursor_unpack_be(csr, &val);
        ASSERT_EQ_FMT(instr->val.i32, val, "%" PRId32);
    } break;
    case it_u64: {
        uint64_t val;
        res = cursor_unpack_be(csr, &val);
        ASSERT_EQ_FMTm(err_msg, instr->val.u64, val, "%" PRIu64);
    } break;
    case it_i64: {
        int64_t val;
        res = cursor_unpack_be(csr, &val);
        ASSERT_EQ_FMTm(err_msg, instr->val.i64, val, "%" PRId64);
    } break;
    case it_f: {
        float val;
        res = cursor_unpack_be(csr, &val);
        ASSERT_EQ_FMTm(err_msg, instr->val.f, val, "%f");
    } break;
    case it_d: {
        double val;
        res = cursor_unpack_be(csr, &val);
        ASSERT_EQ_FMTm(err_msg, instr->val.d, val, "%f");
    } break;
    }
    ASSERT_EQ(cursor_res_ok, res);
    PASS();
}

TEST
cursor_unpack_and_compare_to_instr(struct cursor *            csr,
                                   struct instruction const * instr) {
    TEST res;
    switch (instr->endianness) {
    case little:
        res = cursor_unpack_le_and_compare_to_instr(csr, instr);
        break;
    case big:
        res = cursor_unpack_be_and_compare_to_instr(csr, instr);
        break;
    }
    return res;
}

TEST
generic_pack_unpack_le_u8_should_match(void) {
    uint8_t wrote = random_unique();
    uint8_t buf[sizeof(wrote)];
    pack_le(buf, wrote);
    uint8_t unpack;
    unpack_le(&unpack, buf);
    ASSERT_EQ(wrote, unpack);
    PASS();
}

TEST
generic_pack_unpack_le_i8_should_match(void) {
    int8_t  wrote = random_unique();
    uint8_t buf[sizeof(wrote)];
    pack_le(buf, wrote);
    int8_t unpack;
    unpack_le(&unpack, buf);
    ASSERT_EQ(wrote, unpack);
    PASS();
}

TEST
generic_pack_unpack_le_u16_should_match(void) {
    uint16_t wrote = random_unique();
    uint8_t  buf[sizeof(wrote)];
    pack_le(buf, wrote);
    uint16_t unpack;
    unpack_le(&unpack, buf);
    ASSERT_EQ(wrote, unpack);
    PASS();
}

TEST
generic_pack_unpack_le_i16_should_match(void) {
    int16_t wrote = random_unique();
    uint8_t buf[sizeof(wrote)];
    pack_le(buf, wrote);
    int16_t unpack;
    unpack_le(&unpack, buf);
    ASSERT_EQ(wrote, unpack);
    PASS();
}

TEST
generic_pack_unpack_le_u32_should_match(void) {
    uint32_t wrote = random_unique();
    uint8_t  buf[sizeof(wrote)];
    pack_le(buf, wrote);
    uint32_t unpack;
    unpack_le(&unpack, buf);
    ASSERT_EQ(wrote, unpack);
    PASS();
}

TEST
generic_pack_unpack_le_i32_should_match(void) {
    int32_t wrote = random_unique();
    uint8_t buf[sizeof(wrote)];
    pack_le(buf, wrote);
    int32_t unpack;
    unpack_le(&unpack, buf);
    ASSERT_EQ(wrote, unpack);
    PASS();
}

TEST
generic_pack_unpack_le_u64_should_match(void) {
    uint64_t wrote = ((uint64_t)random() << 32) & random_unique();
    uint8_t  buf[sizeof(wrote)];
    pack_le(buf, wrote);
    uint64_t unpack;
    unpack_le(&unpack, buf);
    ASSERT_EQ(wrote, unpack);
    PASS();
}

TEST
generic_pack_unpack_le_i64_should_match(void) {
    int64_t wrote = ((int64_t)random() << 32) & random_unique();
    uint8_t buf[sizeof(wrote)];
    pack_le(buf, wrote);
    int64_t unpack;
    unpack_le(&unpack, buf);
    ASSERT_EQ(wrote, unpack);
    PASS();
}

TEST
generic_pack_unpack_le_f_should_match(void) {
    float   wrote = random_float();
    uint8_t buf[sizeof(wrote)];
    pack_le(buf, wrote);
    float unpack;
    unpack_le(&unpack, buf);
    ASSERT_EQ_FMT(wrote, unpack, "%f");
    PASS();
}

TEST
generic_pack_unpack_le_d_should_match(void) {
    double  wrote = random_float();
    uint8_t buf[sizeof(wrote)];
    pack_le(buf, wrote);
    double unpack;
    unpack_le(&unpack, buf);
    ASSERT_EQ_FMT(wrote, unpack, "%f");
    PASS();
}

SUITE(generic_pack_unpack_le) {
    RUN_TEST(generic_pack_unpack_le_u8_should_match);
    RUN_TEST(generic_pack_unpack_le_i8_should_match);
    RUN_TEST(generic_pack_unpack_le_u16_should_match);
    RUN_TEST(generic_pack_unpack_le_i16_should_match);
    RUN_TEST(generic_pack_unpack_le_u32_should_match);
    RUN_TEST(generic_pack_unpack_le_i32_should_match);
    RUN_TEST(generic_pack_unpack_le_u64_should_match);
    RUN_TEST(generic_pack_unpack_le_i64_should_match);
    RUN_TEST(generic_pack_unpack_le_f_should_match);
    RUN_TEST(generic_pack_unpack_le_d_should_match);
}

TEST
generic_pack_unpack_be_u8_should_match(void) {
    uint8_t wrote = random_unique();
    uint8_t buf[sizeof(wrote)];
    pack_be(buf, wrote);
    uint8_t unpack;
    unpack_be(&unpack, buf);
    ASSERT_EQ(wrote, unpack);
    PASS();
}

TEST
generic_pack_unpack_be_i8_should_match(void) {
    int8_t  wrote = random_unique();
    uint8_t buf[sizeof(wrote)];
    pack_be(buf, wrote);
    int8_t unpack;
    unpack_be(&unpack, buf);
    ASSERT_EQ(wrote, unpack);
    PASS();
}

TEST
generic_pack_unpack_be_u16_should_match(void) {
    uint16_t wrote = random_unique();
    uint8_t  buf[sizeof(wrote)];
    pack_be(buf, wrote);
    uint16_t unpack;
    unpack_be(&unpack, buf);
    ASSERT_EQ(wrote, unpack);
    PASS();
}

TEST
generic_pack_unpack_be_i16_should_match(void) {
    int16_t wrote = random_unique();
    uint8_t buf[sizeof(wrote)];
    pack_be(buf, wrote);
    int16_t unpack;
    unpack_be(&unpack, buf);
    ASSERT_EQ(wrote, unpack);
    PASS();
}

TEST
generic_pack_unpack_be_u32_should_match(void) {
    uint32_t wrote = random_unique();
    uint8_t  buf[sizeof(wrote)];
    pack_be(buf, wrote);
    uint32_t unpack;
    unpack_be(&unpack, buf);
    ASSERT_EQ(wrote, unpack);
    PASS();
}

TEST
generic_pack_unpack_be_i32_should_match(void) {
    int32_t wrote = random_unique();
    uint8_t buf[sizeof(wrote)];
    pack_be(buf, wrote);
    int32_t unpack;
    unpack_be(&unpack, buf);
    ASSERT_EQ(wrote, unpack);
    PASS();
}

TEST
generic_pack_unpack_be_u64_should_match(void) {
    uint64_t wrote = ((uint64_t)random() << 32) & random_unique();
    uint8_t  buf[sizeof(wrote)];
    pack_be(buf, wrote);
    uint64_t unpack;
    unpack_be(&unpack, buf);
    ASSERT_EQ(wrote, unpack);
    PASS();
}

TEST
generic_pack_unpack_be_i64_should_match(void) {
    int64_t wrote = ((int64_t)random() << 32) & random_unique();
    uint8_t buf[sizeof(wrote)];
    pack_be(buf, wrote);
    int64_t unpack;
    unpack_be(&unpack, buf);
    ASSERT_EQ(wrote, unpack);
    PASS();
}

TEST
generic_pack_unpack_be_f_should_match(void) {
    float   wrote = random_float();
    uint8_t buf[sizeof(wrote)];
    pack_be(buf, wrote);
    float unpack;
    unpack_be(&unpack, buf);
    ASSERT_EQ_FMT(wrote, unpack, "%f");
    PASS();
}

TEST
generic_pack_unpack_be_d_should_match(void) {
    double  wrote = random_float();
    uint8_t buf[sizeof(wrote)];
    pack_be(buf, wrote);
    double unpack;
    unpack_be(&unpack, buf);
    ASSERT_EQ_FMT(wrote, unpack, "%f");
    PASS();
}

SUITE(generic_pack_unpack_be) {
    RUN_TEST(generic_pack_unpack_be_u8_should_match);
    RUN_TEST(generic_pack_unpack_be_i8_should_match);
    RUN_TEST(generic_pack_unpack_be_u16_should_match);
    RUN_TEST(generic_pack_unpack_be_i16_should_match);
    RUN_TEST(generic_pack_unpack_be_u32_should_match);
    RUN_TEST(generic_pack_unpack_be_i32_should_match);
    RUN_TEST(generic_pack_unpack_be_u64_should_match);
    RUN_TEST(generic_pack_unpack_be_i64_should_match);
    RUN_TEST(generic_pack_unpack_be_f_should_match);
    RUN_TEST(generic_pack_unpack_be_d_should_match);
}

TEST
generic_mixed_endian_u16_should_not_match(void) {
    uint16_t wrote = random_unique();
    uint8_t  buf[sizeof(wrote)];
    pack_le(buf, wrote);
    uint16_t unpack;
    unpack_be(&unpack, buf);
    ASSERT_FALSE(wrote == unpack);
    pack_be(buf, wrote);
    unpack_le(&unpack, buf);
    ASSERT_FALSE(wrote == unpack);
    PASS();
}

TEST
generic_mixed_endian_i16_should_not_match(void) {
    int16_t wrote = random_unique();
    uint8_t buf[sizeof(wrote)];
    pack_le(buf, wrote);
    int16_t unpack;
    unpack_be(&unpack, buf);
    ASSERT_FALSE(wrote == unpack);
    pack_be(buf, wrote);
    unpack_le(&unpack, buf);
    ASSERT_FALSE(wrote == unpack);
    PASS();
}

TEST
generic_mixed_endian_u32_should_not_match(void) {
    uint32_t wrote = random_unique();
    uint8_t  buf[sizeof(wrote)];
    pack_le(buf, wrote);
    uint32_t unpack;
    unpack_be(&unpack, buf);
    ASSERT_FALSE(wrote == unpack);
    pack_be(buf, wrote);
    unpack_le(&unpack, buf);
    ASSERT_FALSE(wrote == unpack);
    PASS();
}

TEST
generic_mixed_endian_i32_should_not_match(void) {
    int32_t wrote = random_unique();
    uint8_t buf[sizeof(wrote)];
    pack_le(buf, wrote);
    int32_t unpack;
    unpack_be(&unpack, buf);
    ASSERT_FALSE(wrote == unpack);
    pack_be(buf, wrote);
    unpack_le(&unpack, buf);
    ASSERT_FALSE(wrote == unpack);
    PASS();
}

TEST
generic_mixed_endian_u64_should_not_match(void) {
    uint64_t wrote = random_unique();
    uint8_t  buf[sizeof(wrote)];
    pack_le(buf, wrote);
    uint64_t unpack;
    unpack_be(&unpack, buf);
    ASSERT_FALSE(wrote == unpack);
    pack_be(buf, wrote);
    unpack_le(&unpack, buf);
    ASSERT_FALSE(wrote == unpack);
    PASS();
}

TEST
generic_mixed_endian_i64_should_not_match(void) {
    int64_t wrote = random_unique();
    uint8_t buf[sizeof(wrote)];
    pack_le(buf, wrote);
    int64_t unpack;
    unpack_be(&unpack, buf);
    ASSERT_FALSE(wrote == unpack);
    pack_be(buf, wrote);
    unpack_le(&unpack, buf);
    ASSERT_FALSE(wrote == unpack);
    PASS();
}

TEST
generic_mixed_endian_f_should_not_match(void) {
    float   wrote = random_unique();
    uint8_t buf[sizeof(wrote)];
    pack_le(buf, wrote);
    float unpack;
    unpack_be(&unpack, buf);
    ASSERT_FALSE(wrote == unpack);
    pack_be(buf, wrote);
    unpack_le(&unpack, buf);
    ASSERT_FALSE(wrote == unpack);
    PASS();
}

TEST
generic_mixed_endian_d_should_not_match(void) {
    double  wrote = random_float();
    uint8_t buf[sizeof(wrote)];
    pack_le(buf, wrote);
    double unpack;
    unpack_be(&unpack, buf);
    ASSERT_FALSE(wrote == unpack);
    pack_be(buf, wrote);
    unpack_le(&unpack, buf);
    ASSERT_FALSE(wrote == unpack);
    PASS();
}

#define TEST_PLAN_SIZE 100000
struct instruction is[TEST_PLAN_SIZE];
uint8_t            test_buf[TEST_PLAN_SIZE * sizeof(uint64_t)];

TEST
roundtrip_plan_shoud_match() {
    /* Make a unpack/pack plan */
    struct plan plan = random_plan(is, TEST_PLAN_SIZE);

    /* Create a cursor object which will manage writing into the buffer */
    struct cursor csr = cursor_new(test_buf, plan.req_buf_size);

    /* Iterate through the plan and encode it's values into the buffer */
    for (size_t i = 0; i < plan.nis; i++) {
        enum cursor_res res = cursor_pack_instr(&csr, &plan.is[i]);
        ASSERT_EQ_FMTm("cursor_pack_le returned an error", cursor_res_ok, res, "%d");
    }
    ASSERT_EQ_FMTm("Position and len do not match", csr.pos, csr.len, "%d");

    /* Create a cursor object which will manage unpacking out of the buffer */
    csr = cursor_new(test_buf, plan.req_buf_size);

    /* Iterate through the plan, unpack values out of the csr, and make sure
     * they
     * match the plan's instruction values */
    for (size_t i = 0; i < plan.nis; i++) {
        struct instruction const * instr = &plan.is[i];
        GREATEST_CHECK_CALL(cursor_unpack_and_compare_to_instr(&csr, instr));
    }
    PASS();
}

SUITE(generic_mixed_endian) {
    RUN_TEST(generic_mixed_endian_u16_should_not_match);
    RUN_TEST(generic_mixed_endian_i16_should_not_match);
    RUN_TEST(generic_mixed_endian_u32_should_not_match);
    RUN_TEST(generic_mixed_endian_i32_should_not_match);
    RUN_TEST(generic_mixed_endian_u64_should_not_match);
    RUN_TEST(generic_mixed_endian_i64_should_not_match);
    RUN_TEST(generic_mixed_endian_f_should_not_match);
    RUN_TEST(generic_mixed_endian_d_should_not_match);
}

SUITE(plan) {
    RUN_TEST(roundtrip_plan_shoud_match);
}
GREATEST_MAIN_DEFS();

int
main(int argc, char ** argv) {
    srandom(time(NULL));
    GREATEST_MAIN_BEGIN();
    RUN_SUITE(generic_pack_unpack_le);
    RUN_SUITE(generic_pack_unpack_be);
    RUN_SUITE(generic_mixed_endian);
    RUN_SUITE(plan);
    GREATEST_MAIN_END();
}
