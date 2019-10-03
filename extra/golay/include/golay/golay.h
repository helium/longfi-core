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

#ifndef GOLAY_6DA96514
#define GOLAY_6DA96514

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Encodes 12 data bits into 24 coded bits.
 */
void
golay_encode12(uint16_t const in, uint8_t * out);

/**
 * Decodes 24 coded bits into 12 data bits.
 *
 * @retval number of corrected bits.
 * @retval -1 on failure to decode.
 */
int
golay_decode12(uint8_t const * in, uint16_t * out);

int
popcount_2(uint32_t x);


#ifdef __cplusplus
}
#endif

#endif /* GOLAY_6DA96514 */
