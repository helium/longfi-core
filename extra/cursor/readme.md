[![Build Status](https://travis-ci.org/JayKickliter/cursor.svg?branch=master)](https://travis-ci.org/JayKickliter/cursor)
[![Coverage Status](https://coveralls.io/repos/github/JayKickliter/cursor/badge.svg?branch=master)](https://coveralls.io/github/JayKickliter/cursor?branch=master)

# `cursor`

A simple iterator for packing/unpacking primitive c types to/from a provided buffer.

## Example

```c
    /* A buffer to store packed data */
    uint8_t buf[6];

    /* Construct a new cursor with the buffer we just defined */
    struct cursor csr = cursor_new(buf, sizeof(buf));

    uint16_t written_u16 = 0x0123;
    uint32_t written_u32 = 0x456789AB;

    /* Write written_u16 to buf little-endian */
    cursor_pack_le(&csr, written_u16);

    /* Write written_u32 to buf big-endian */
    cursor_pack_be(&csr, written_u32);

    /* Reset csr */
    csr = cursor_new(buf, sizeof(buf));

    uint16_t read_u16 = 0;
    uint32_t read_u32 = 0;

    /* Read a packed little-endian uint16_t out of buf */
    cursor_unpack_le(&csr, &read_u16);

    /* Read a packed big-endian uint32_t out of buf */
    cursor_unpack_be(&csr, &read_u32);

    assert(read_u16 == written_u16);
    assert(read_u32 == written_u32);
```

See the full [example](examples/simple.c).

## Prerequisites

`cursor` makes use of c11's generic selection expression feature. These are the minimum `gcc` and `clang` versions that support it:

* clang >= 3.8
* gcc  >= 4.9

Feel free to open an issue if you'd like to use `cursor` but you're compiler is not compatible. The generic pack/unpack function calls are just sugar, and a configuration step could `ifdef` them out when not supported.
