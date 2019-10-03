// -*- Mode: C; c-basic-offset: 8; -*-
/*
 * Copyright (c) 2007 - 2015 Joseph Gaeddert (https://github.com/jgaeddert/liquid-dsp)
 * Copyright (c) 2018 Bart Massey (https://github.com/BartMassey/popcount)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <assert.h>
#include <stdint.h>

// P matrix [12 x 12]
uint32_t golay2412_P[12] = {0x08ed,
                            0x01db,
                            0x03b5,
                            0x0769,
                            0x0ed1,
                            0x0da3,
                            0x0b47,
                            0x068f,
                            0x0d1d,
                            0x0a3b,
                            0x0477,
                            0x0ffe};

// generator matrix transposed [24 x 12]
uint32_t golay2412_Gt[24] = {0x08ed, 0x01db, 0x03b5, 0x0769, 0x0ed1, 0x0da3,
                             0x0b47, 0x068f, 0x0d1d, 0x0a3b, 0x0477, 0x0ffe,
                             0x0800, 0x0400, 0x0200, 0x0100, 0x0080, 0x0040,
                             0x0020, 0x0010, 0x0008, 0x0004, 0x0002, 0x0001};

// parity check matrix [12 x 24]
uint32_t golay2412_H[12] = {0x008008ed,
                            0x004001db,
                            0x002003b5,
                            0x00100769,
                            0x00080ed1,
                            0x00040da3,
                            0x00020b47,
                            0x0001068f,
                            0x00008d1d,
                            0x00004a3b,
                            0x00002477,
                            0x00001ffe};

int
popcount_2(uint32_t x) {
    uint32_t m1 = 0x55555555;
    uint32_t m2 = 0x33333333;
    uint32_t m4 = 0x0f0f0f0f;
    x -= (x >> 1) & m1;
    x = (x & m2) + ((x >> 2) & m2);
    x = (x + (x >> 4)) & m4;
    x += x >> 8;
    return (x + (x >> 16)) & 0x3f;
}


// multiply input vector with parity check matrix, H
uint32_t
golay2412_matrix_mul(uint32_t _v, uint32_t * _A, uint8_t _n) {
    unsigned int x = 0;
    unsigned int i;
    for (i = 0; i < _n; i++) {
        x <<= 1;
        unsigned int c = 0;
        unsigned int p = _A[i] & _v;
        c += popcount_2(p & 0xff);
        p >>= 8;
        c += popcount_2(p & 0xff);
        p >>= 8;
        c += popcount_2(p & 0xff);

        x |= c & 0x0001; // mod 2
    }
    return x;
}

// search for p[i] such that w(v+p[i]) <= 2, return -1 on fail
int
golay2412_parity_search(unsigned int _v) {
    assert(_v < (1 << 12));

    unsigned int i;
    for (i = 0; i < 12; i++) {
        // same as above but faster, exploiting fact that P has
        // only 12 bits of resolution
        unsigned int wj = 0;
        unsigned int p  = _v ^ golay2412_P[i];
        wj += popcount_2((p)&0xff);
        wj += popcount_2((p >> 8) & 0xff);
        if (wj <= 2)
            return i;
    }

    // could not find p[i] to satisfy criteria
    return -1;
}


uint32_t
fec_golay2412_encode_symbol(uint16_t const _sym_dec) {
    // validate input
    assert(_sym_dec < (1 << 12));

    // compute encoded/transmitted message: v = m*G
    return golay2412_matrix_mul(_sym_dec, golay2412_Gt, 24);
}

int
fec_golay2412_decode_symbol(uint32_t _sym_enc, uint16_t * out) {
    // validate input
    assert(_sym_enc < (1 << 24));

    // state variables
    unsigned int s     = 0; // syndrome vector
    unsigned int e_hat = 0; // estimated error vector
    unsigned int v_hat = 0; // estimated transmitted message

    // compute syndrome vector, s = r*H^T = ( H*r^T )^T
    s = golay2412_matrix_mul(_sym_enc, golay2412_H, 12);

    // compute weight of s (12 bits)
    unsigned int ws = popcount_2(s);

    // step 2:
    e_hat = 0;
    if (ws <= 3) {
        // set e_hat = [s 0(12)]
        e_hat = (s << 12) & 0xfff000;
    } else {
        // step 3: search for p[i] s.t. w(s+p[i]) <= 2
        int s_index = golay2412_parity_search(s);

        if (s_index >= 0) {
            // vector found!
            // NOTE : uj = 1 << (12-j-1)
            e_hat = ((s ^ golay2412_P[s_index]) << 12) | (1 << (11 - s_index));
        } else {
            // step 4: compute s*P
            unsigned int sP = golay2412_matrix_mul(s, golay2412_P, 12);

            // compute weight of sP (12 bits)
            unsigned int wsP = popcount_2(sP);

            if (wsP == 2 || wsP == 3) {
                // step 5: set e = [0, s*P]
                e_hat = sP;
            } else {
                // step 6: search for p[i] s.t. w(s*P + p[i]) == 2...

                int sP_index = golay2412_parity_search(sP);

                if (sP_index >= 0) {
                    // vector found!
                    // NOTE : uj = 1 << (12-j-1)
                    //      [      uj << 1 2    ] [    sP + p[j]    ]
                    e_hat = (1 << (23 - sP_index)) | (sP ^ golay2412_P[sP_index]);
                } else {
                    // step 7: decoding error
                    return -1;
                }
            }
        }
    }

    // step 8: compute estimated transmitted message: v_hat = r + e_hat
    v_hat = _sym_enc ^ e_hat;
    // compute estimated original message: (last 12 bits of encoded message)
    *out = v_hat & 0x0fff;

    return popcount_2(e_hat);
}

// encode 12 bits data into 24 bits of coded data
// input is in scratch_3[], output in scratch_6[]
void
golay_encode12(uint16_t const in, uint8_t * out) {
    // top 4 bits should all be 0s
    assert((in & 0xF000) == 0);

    uint32_t res = fec_golay2412_encode_symbol(in);

    out[0] = res & 0xFF;
    res >>= 8;
    out[1] = res & 0xFF;
    res >>= 8;
    out[2] = res & 0xFF;
}

// decode 24 bits of coded data into 12 bits of original data
// returns number of corrected bits or -1 on decode failure
int
golay_decode12(uint8_t const * in, uint16_t * out) {
    uint32_t input = (in[2] << 16) | (in[1] << 8) | in[0];
    return fec_golay2412_decode_symbol(input, out);
}
