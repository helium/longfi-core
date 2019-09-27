// -*- Mode: C; c-basic-offset: 8; -*-
//
// Copyright (c) 2012 Andrew Tridgell, All Rights Reserved
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
//  o Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  o Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in
//    the documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.
//

///
/// @file	golay.c
///
/// golay 23/12 error correction encoding and decoding
///

#include <assert.h>
#include <golay/golay.h>

extern const uint16_t golay23_encode[4096];
extern const uint16_t golay23_decode[2048];

// encode 3 bytes data into 6 bytes of coded data
// input is in scratch_3[], output in scratch_6[]
static void
golay_encode24(uint8_t const * in, uint8_t * out) {
    uint16_t v;
    uint16_t syn;

    v      = in[0] | ((uint16_t)in[1] & 0x0F) << 8;
    syn    = golay23_encode[v];
    out[0] = syn & 0xFF;
    out[1] = (in[0] & 0x1F) << 3 | syn >> 8;
    out[2] = (in[0] & 0xE0) >> 5 | (in[1] & 0x0F) << 3;

    v      = in[2] | ((uint16_t)in[1] & 0xF0) << 4;
    syn    = golay23_encode[v];
    out[3] = syn & 0xFF;
    out[4] = (in[2] & 0x1F) << 3 | syn >> 8;
    out[5] = (in[2] & 0xE0) >> 5 | (in[1] & 0xF0) >> 1;
}

// encode n bytes of data into 2n coded bytes. n must be a multiple 3
// encoding takes about 6 microseconds per input byte
void
golay_encode(uint8_t n, uint8_t const * in, uint8_t * out) {
    assert(n % 3 == 0);

    while (n >= 3) {
        golay_encode24(in, out);
        in += 3;
        out += 6;
        n -= 3;
    }
}

// decode 6 bytes of coded data into 3 bytes of original data
// input is in scratch_6[], output in scratch_3[]
// returns the number of words corrected (0, 1 or 2)
static uint8_t
golay_decode24(uint8_t const * in, uint8_t * out) {
    uint16_t v;
    uint16_t syn;
    uint16_t e;
    uint8_t  errcount = 0;

    v   = (in[2] & 0x7F) << 5 | (in[1] & 0xF8) >> 3;
    syn = golay23_encode[v];
    syn ^= in[0] | (in[1] & 0x07) << 8;
    e = golay23_decode[syn];
    if (e) {
        errcount++;
        v ^= e;
    }
    out[0] = v & 0xFF;
    out[1] = v >> 8;

    v   = (in[5] & 0x7F) << 5 | (in[4] & 0xF8) >> 3;
    syn = golay23_encode[v];
    syn ^= in[3] | (in[4] & 0x07) << 8;
    e = golay23_decode[syn];
    if (e) {
        errcount++;
        v ^= e;
    }
    out[1] |= (v >> 4) & 0xF0;
    out[2] = v & 0xFF;

    return errcount;
}

// decode n bytes of coded data into n/2 bytes of original data
// n must be a multiple of 6
// decoding takes about 4 microseconds per input byte
// the number of 12 bit words that required correction is returned
uint8_t
golay_decode(uint8_t n, uint8_t const * in, uint8_t * out) {
    assert(n % 6 == 0);

    uint8_t errcount = 0;

    while (n >= 6) {
        errcount += golay_decode24(in, out);
        in += 6;
        out += 3;
        n -= 6;
    }
    return errcount;
}
