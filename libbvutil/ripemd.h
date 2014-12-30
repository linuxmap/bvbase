/*
 * Copyright (C) 2007 Michael Niedermayer <michaelni@gmx.at>
 * Copyright (C) 2013 James Almer <jamrial@gmail.com>
 *
 * This file is part of BVbase.
 *
 * BVbase is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * BVbase is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BVbase; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef BVUTIL_RIPEMD_H
#define BVUTIL_RIPEMD_H

#include <stdint.h>

#include "attributes.h"
#include "version.h"

/**
 * @defgroup lavu_ripemd RIPEMD
 * @ingroup lavu_crypto
 * @{
 */

extern const int bv_ripemd_size;

struct BVRIPEMD;

/**
 * Allocate an BVRIPEMD context.
 */
struct BVRIPEMD *bv_ripemd_alloc(void);

/**
 * Initialize RIPEMD hashing.
 *
 * @param context pointer to the function context (of size bv_ripemd_size)
 * @param bits    number of bits in digest (128, 160, 256 or 320 bits)
 * @return        zero if initialization succeeded, -1 otherwise
 */
int bv_ripemd_init(struct BVRIPEMD* context, int bits);

/**
 * Update hash value.
 *
 * @param context hash function context
 * @param data    input data to update hash with
 * @param len     input data length
 */
void bv_ripemd_update(struct BVRIPEMD* context, const uint8_t* data, unsigned int len);

/**
 * Finish hashing and output digest value.
 *
 * @param context hash function context
 * @param digest  buffer where output digest value is stored
 */
void bv_ripemd_final(struct BVRIPEMD* context, uint8_t *digest);

/**
 * @}
 */

#endif /* BVUTIL_RIPEMD_H */