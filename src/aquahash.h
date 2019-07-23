// Copyright 2018 J. Andrew Rogers
// Copyright 2019 Hung Dang
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <cassert>
#include <cstdint>
#include <cstring>
#include <immintrin.h>
#include <limits>
#include <stdint.h>

class AquaHash {
  private:
    static constexpr unsigned int THRESHOLD = 64;
    static constexpr size_t MAX_INPUT = std::numeric_limits<size_t>::max() - 1;
    // sentinel to prevent double finalization
    static constexpr size_t FINALIZED = MAX_INPUT + 1;

    // 64 bit constants
    enum Constants64Bits : uint64_t {
        CONSTANT_64_1 = 0xa11202c9b468bea1,
        CONSTANT_64_2 = 0xd75157a01452495b,
        CONSTANT_64_3 = 0xb1293b3305418592,
        CONSTANT_64_4 = 0xd210d232c6429b69,
        CONSTANT_64_5 = 0xbd3dc2b7b87c4715,
        CONSTANT_64_6 = 0x6a6c9527ac2e0e4e,
        CONSTANT_64_7 = 0xcc96ed1674eaaa03,
        CONSTANT_64_8 = 0x1e863f24b2a8316a,
        CONSTANT_64_9 = 0x8e51ef21fabb4522,
        CONSTANT_64_10 = 0xe43d7a0656954b6c,
        CONSTANT_64_11 = 0x56082007c71ab18f,
        CONSTANT_64_12 = 0x76435569a03af7fa,
        CONSTANT_64_13 = 0xd2600de7157abc68,
        CONSTANT_64_14 = 0x6339e901c3031efb,
    };

    static constexpr uint32_t MASK_32BITS = 0xffffffff;
    static constexpr uint32_t MASK_16BITS = 0xffff;
    static constexpr uint32_t MASK_8BITS = 0xff;

    // 32 bit constants
    enum Constant32Bits : uint32_t {
        CONSTANT_32_1 = (CONSTANT_64_3 >> 32) & MASK_32BITS,
        CONSTANT_32_2 = CONSTANT_64_3 & MASK_32BITS,
        CONSTANT_32_3 = (CONSTANT_64_4 >> 32) & MASK_32BITS,
    };

    // 16 bit constants
    enum Constant16Bits {
        CONSTANT_16_1 = (CONSTANT_64_5 >> 48) & MASK_16BITS, // 0xbd3d
        CONSTANT_16_2 = (CONSTANT_64_5 >> 32) & MASK_16BITS, // 0xc2b7
        CONSTANT_16_3 = (CONSTANT_64_5 >> 16) & MASK_16BITS, // 0xb87c
        CONSTANT_16_4 = CONSTANT_64_5 & MASK_16BITS,         // 0x4715
        CONSTANT_16_5 = (CONSTANT_64_6 >> 48) & MASK_16BITS, // 0x6a6c
        CONSTANT_16_6 = (CONSTANT_64_6 >> 32) & MASK_16BITS, // 0x9527
        CONSTANT_16_7 = (CONSTANT_64_6 >> 16) & MASK_16BITS, // 0xac2e
    };

    // 8 bit constants
    enum Constant8Bits {
        CONSTANT_8_01 = (CONSTANT_64_7 >> 56) & MASK_8BITS, // 0xcc
        CONSTANT_8_02 = (CONSTANT_64_7 >> 48) & MASK_8BITS, // 0x96
        CONSTANT_8_03 = (CONSTANT_64_7 >> 40) & MASK_8BITS, // 0xed,
        CONSTANT_8_04 = (CONSTANT_64_7 >> 32) & MASK_8BITS, // 0x16,
        CONSTANT_8_05 = (CONSTANT_64_7 >> 24) & MASK_8BITS, // 0x74,
        CONSTANT_8_06 = (CONSTANT_64_7 >> 16) & MASK_8BITS, // 0xea,
        CONSTANT_8_07 = (CONSTANT_64_7 >> 8) & MASK_8BITS,  // 0xaa,
        CONSTANT_8_08 = (CONSTANT_64_7)&MASK_8BITS,         // 0x03,
        CONSTANT_8_09 = (CONSTANT_64_8 >> 56) & MASK_8BITS, // 0x1e,
        CONSTANT_8_10 = (CONSTANT_64_8 >> 48) & MASK_8BITS, // 0x86,
        CONSTANT_8_11 = (CONSTANT_64_8 >> 40) & MASK_8BITS, // 0x3f,
        CONSTANT_8_12 = (CONSTANT_64_8 >> 32) & MASK_8BITS, // 0x24,
        CONSTANT_8_13 = (CONSTANT_64_8 >> 24) & MASK_8BITS, // 0xb2,
        CONSTANT_8_14 = (CONSTANT_64_8 >> 16) & MASK_8BITS, // 0xa8,
        CONSTANT_8_15 = (CONSTANT_64_8 >> 8) & MASK_8BITS,  // 0x31,
    };

    // 4 x 128-bit hashing lanes
    __m128i block[4];

    // input block buffer
    __m128i input[4];

    // initialization vector
    __m128i initialize;

    // cumulative input bytes
    size_t input_bytes;

  public:
    // Reference implementation of AquaHash small key algorithm
    static __m128i SmallKeyAlgorithm(const uint8_t *key, const size_t bytes,
                                     __m128i initialize = _mm_setzero_si128()) {
        assert(bytes <= MAX_INPUT);
        __m128i hash = initialize;

        // bulk hashing loop -- 128-bit block size
        const __m128i *ptr128 = reinterpret_cast<const __m128i *>(key);
        if (bytes / sizeof(hash)) {
            __m128i temp = _mm_set_epi64x(CONSTANT_64_1, CONSTANT_64_2);
            for (uint32_t i = 0; i < bytes / sizeof(hash); ++i) {
                __m128i b = _mm_loadu_si128(ptr128++);
                hash = _mm_aesenc_si128(hash, b);
                temp = _mm_aesenc_si128(temp, b);
            }
            hash = _mm_aesenc_si128(hash, temp);
        }

        // AES sub-block processor
        const uint8_t *ptr8 = reinterpret_cast<const uint8_t *>(ptr128);
        if (bytes & 8) {
            __m128i b = _mm_set_epi64x(*reinterpret_cast<const uint64_t *>(ptr8), CONSTANT_64_1);
            hash = _mm_xor_si128(hash, b);
            ptr8 += 8;
        }

        if (bytes & 4) {
            __m128i b = _mm_set_epi32(CONSTANT_32_1, CONSTANT_32_2,
                                      *reinterpret_cast<const uint32_t *>(ptr8), CONSTANT_32_3);
            hash = _mm_xor_si128(hash, b);
            ptr8 += 4;
        }

        if (bytes & 2) {
            __m128i b =
                _mm_set_epi16(CONSTANT_16_1, CONSTANT_16_2, CONSTANT_16_3, CONSTANT_16_4, CONSTANT_16_5,
                              CONSTANT_16_6, *reinterpret_cast<const uint16_t *>(ptr8), CONSTANT_16_7);
            hash = _mm_xor_si128(hash, b);
            ptr8 += 2;
        }

        if (bytes & 1) {
            __m128i b = _mm_set_epi8(
                CONSTANT_8_01, CONSTANT_8_02, CONSTANT_8_03, CONSTANT_8_04, CONSTANT_8_05, CONSTANT_8_06,
                CONSTANT_8_07, CONSTANT_8_08, CONSTANT_8_09, CONSTANT_8_10, CONSTANT_8_11, CONSTANT_8_12,
                CONSTANT_8_13, CONSTANT_8_14, *reinterpret_cast<const uint8_t *>(ptr8), CONSTANT_8_15);
            hash = _mm_xor_si128(hash, b);
        }

        // this algorithm construction requires no less than three AES rounds to finalize
        hash = _mm_aesenc_si128(hash, _mm_set_epi64x(CONSTANT_64_9, CONSTANT_64_10));
        hash = _mm_aesenc_si128(hash, _mm_set_epi64x(CONSTANT_64_11, CONSTANT_64_12));
        return _mm_aesenc_si128(hash, _mm_set_epi64x(CONSTANT_64_13, CONSTANT_64_14));
    }

    // Reference implementation of AquaHash large key algorithm
    static __m128i LargeKeyAlgorithm(const uint8_t *key, const size_t bytes,
                                     __m128i initialize = _mm_setzero_si128()) {
        assert(bytes <= MAX_INPUT);

        // initialize 4 x 128-bit hashing lanes, for a 512-bit block size
        __m128 block[4] = {_mm_xor_si128(initialize, _mm_set_epi64x(CONSTANT_64_1, CONSTANT_64_2)),
                           _mm_xor_si128(initialize, _mm_set_epi64x(CONSTANT_64_3, CONSTANT_64_4)),
                           _mm_xor_si128(initialize, _mm_set_epi64x(CONSTANT_64_5, CONSTANT_64_6)),
                           _mm_xor_si128(initialize, _mm_set_epi64x(CONSTANT_64_7, CONSTANT_64_8))};

        // bulk hashing loop -- 512-bit block size
        const __m128i *ptr128 = reinterpret_cast<const __m128i *>(key);
        for (size_t block_counter = 0; block_counter < bytes / sizeof(block); block_counter++) {
            block[0] = _mm_aesenc_si128(block[0], _mm_loadu_si128(ptr128++));
            block[1] = _mm_aesenc_si128(block[1], _mm_loadu_si128(ptr128++));
            block[2] = _mm_aesenc_si128(block[2], _mm_loadu_si128(ptr128++));
            block[3] = _mm_aesenc_si128(block[3], _mm_loadu_si128(ptr128++));
        }

        // process remaining AES blocks
        if (bytes & 32) {
            block[0] = _mm_aesenc_si128(block[0], _mm_loadu_si128(ptr128++));
            block[1] = _mm_aesenc_si128(block[1], _mm_loadu_si128(ptr128++));
        }

        if (bytes & 16) {
            block[2] = _mm_aesenc_si128(block[2], _mm_loadu_si128(ptr128++));
        }

        // AES sub-block processor
        const uint8_t *ptr8 = reinterpret_cast<const uint8_t *>(ptr128);
        if (bytes & 8) {
            __m128i b = _mm_set_epi64x(*reinterpret_cast<const uint64_t *>(ptr8), CONSTANT_64_1);
            block[3] = _mm_aesenc_si128(block[3], b);
            ptr8 += 8;
        }

        if (bytes & 4) {
            __m128i b = _mm_set_epi32(CONSTANT_32_1, CONSTANT_32_2,
                                      *reinterpret_cast<const uint32_t *>(ptr8), CONSTANT_32_3);
            block[0] = _mm_aesenc_si128(block[0], b);
            ptr8 += 4;
        }

        if (bytes & 2) {
            __m128i b =
                _mm_set_epi16(CONSTANT_16_1, CONSTANT_16_2, CONSTANT_16_3, CONSTANT_16_4, CONSTANT_16_5,
                              CONSTANT_16_6, *reinterpret_cast<const uint16_t *>(ptr8), CONSTANT_16_7);
            block[1] = _mm_aesenc_si128(block[1], b);
            ptr8 += 2;
        }

        if (bytes & 1) {
            __m128i b = _mm_set_epi8(CONSTANT_8_01, CONSTANT_8_02, CONSTANT_8_03, CONSTANT_8_04,
                                     CONSTANT_8_05, CONSTANT_8_06, CONSTANT_8_07, CONSTANT_8_08,
                                     CONSTANT_8_09, CONSTANT_8_10, CONSTANT_8_11, CONSTANT_8_12,
                                     CONSTANT_8_13, CONSTANT_8_14, *ptr8, CONSTANT_8_15);
            block[2] = _mm_aesenc_si128(block[2], b);
        }

        // indirectly mix hashing lanes
        const __m128i mix =
            _mm_xor_si128(_mm_xor_si128(block[0], block[1]), _mm_xor_si128(block[2], block[3]));
        block[0] = _mm_aesenc_si128(block[0], mix);
        block[1] = _mm_aesenc_si128(block[1], mix);
        block[2] = _mm_aesenc_si128(block[2], mix);
        block[3] = _mm_aesenc_si128(block[3], mix);

        // reduction from 512-bit block size to 128-bit hash
        __m128i hash =
            _mm_aesenc_si128(_mm_aesenc_si128(block[0], block[1]), _mm_aesenc_si128(block[2], block[3]));

        // this algorithm construction requires no less than one round to finalize
        return _mm_aesenc_si128(hash, _mm_set_epi64x(CONSTANT_64_9, CONSTANT_64_10));
    }

    // NON-INCREMENTAL HYBRID ALGORITHM

    static __m128i Hash(const uint8_t *key, const size_t bytes, __m128i initialize = _mm_setzero_si128()) {
        return bytes < THRESHOLD ? SmallKeyAlgorithm(key, bytes, initialize)
                                 : LargeKeyAlgorithm(key, bytes, initialize);
    }

    // INCREMENTAL HYBRID ALGORITHM

    // Initialize a new incremental hashing object
    AquaHash(const __m128i initialize = _mm_setzero_si128())
        : block{_mm_xor_si128(initialize, _mm_set_epi64x(CONSTANT_64_1, CONSTANT_64_2)),
                _mm_xor_si128(initialize, _mm_set_epi64x(CONSTANT_64_3, CONSTANT_64_4)),
                _mm_xor_si128(initialize, _mm_set_epi64x(CONSTANT_64_5, CONSTANT_64_6)),
                _mm_xor_si128(initialize, _mm_set_epi64x(CONSTANT_64_7, CONSTANT_64_8))},
          initialize(initialize),
          input_bytes(0) {}

    // Initialize an existing hashing object -- all previous state is destroyed
    void Initialize(const __m128i initialize = _mm_setzero_si128()) {
        this->initialize = initialize;
        this->input_bytes = 0;
        block[0] = _mm_xor_si128(initialize, _mm_set_epi64x(CONSTANT_64_1, CONSTANT_64_2));
        block[1] = _mm_xor_si128(initialize, _mm_set_epi64x(CONSTANT_64_3, CONSTANT_64_4));
        block[2] = _mm_xor_si128(initialize, _mm_set_epi64x(CONSTANT_64_5, CONSTANT_64_6));
        block[3] = _mm_xor_si128(initialize, _mm_set_epi64x(CONSTANT_64_7, CONSTANT_64_8));
    }

    // Append key to existing hashing object state
    void Update(const uint8_t *key, size_t bytes) {
        assert(input_bytes != FINALIZED);
        assert(bytes <= MAX_INPUT && MAX_INPUT - input_bytes >= bytes);

        if (bytes == 0) return;

        // input buffer may be partially filled
        if (input_bytes % sizeof(input)) {
            // pointer to first unused byte in input buffer
            uint8_t *ptr8 = reinterpret_cast<uint8_t *>(input) + (input_bytes % sizeof(input));

            // compute initial copy size from key to input buffer
            size_t copy_size = sizeof(input) - (input_bytes % sizeof(input));
            if (copy_size > bytes) copy_size = bytes;

            // append new key bytes to input buffer
            memcpy(ptr8, key, copy_size);
            input_bytes += copy_size;
            bytes -= copy_size;

            // input buffer not filled by update
            if (input_bytes % sizeof(input)) return;

            // update key pointer to first byte not in the input buffer
            key += copy_size;

            // hash input buffer
            block[0] = _mm_aesenc_si128(block[0], input[0]);
            block[1] = _mm_aesenc_si128(block[1], input[1]);
            block[2] = _mm_aesenc_si128(block[2], input[2]);
            block[3] = _mm_aesenc_si128(block[3], input[3]);
        }

        input_bytes += bytes;

        // input buffer is empty
        const __m128i *ptr128 = reinterpret_cast<const __m128i *>(key);
        while (bytes >= sizeof(block)) {
            block[0] = _mm_aesenc_si128(block[0], _mm_loadu_si128(ptr128++));
            block[1] = _mm_aesenc_si128(block[1], _mm_loadu_si128(ptr128++));
            block[2] = _mm_aesenc_si128(block[2], _mm_loadu_si128(ptr128++));
            block[3] = _mm_aesenc_si128(block[3], _mm_loadu_si128(ptr128++));
            bytes -= sizeof(block);
        }

        // load remaining bytes into input buffer
        if (bytes) memcpy(input, ptr128, bytes);
    }

    // Generate hash from hashing object state. After finalization, the hashing
    // object is in an undefined state and must be initialized before any
    // subsequent calls on the object.
    __m128i Finalize() {
        assert(input_bytes != FINALIZED);
        if (input_bytes < sizeof(block)) {
            __m128i hash = SmallKeyAlgorithm(reinterpret_cast<uint8_t *>(input), input_bytes, initialize);
            input_bytes = FINALIZED;
            return hash;
        } else {
            // process remaining AES blocks
            if (input_bytes & 32) {
                block[0] = _mm_aesenc_si128(block[0], input[0]);
                block[1] = _mm_aesenc_si128(block[1], input[1]);
            }

            if (input_bytes & 16) {
                block[2] = _mm_aesenc_si128(block[2], input[2]);
            }

            // AES sub-block processor
            const uint8_t *ptr8 = reinterpret_cast<const uint8_t *>(&input[3]);
            if (input_bytes & 8) {
                __m128i b = _mm_set_epi64x(*reinterpret_cast<const uint64_t *>(ptr8), CONSTANT_64_1);
                block[3] = _mm_aesenc_si128(block[3], b);
                ptr8 += 8;
            }

            if (input_bytes & 4) {
                __m128i b = _mm_set_epi32(CONSTANT_32_1, CONSTANT_32_2,
                                          *reinterpret_cast<const uint32_t *>(ptr8), CONSTANT_32_3);
                block[0] = _mm_aesenc_si128(block[0], b);
                ptr8 += 4;
            }

            if (input_bytes & 2) {
                __m128i b =
                    _mm_set_epi16(CONSTANT_16_1, CONSTANT_16_2, CONSTANT_16_3, CONSTANT_16_4, CONSTANT_16_5,
                                  CONSTANT_16_6, *reinterpret_cast<const uint16_t *>(ptr8), CONSTANT_16_7);
                block[1] = _mm_aesenc_si128(block[1], b);
                ptr8 += 2;
            }

            if (input_bytes & 1) {
                __m128i b = _mm_set_epi8(CONSTANT_8_01, CONSTANT_8_02, CONSTANT_8_03, CONSTANT_8_04,
                                         CONSTANT_8_05, CONSTANT_8_06, CONSTANT_8_07, CONSTANT_8_08,
                                         CONSTANT_8_09, CONSTANT_8_10, CONSTANT_8_11, CONSTANT_8_12,
                                         CONSTANT_8_13, CONSTANT_8_14, *ptr8, CONSTANT_8_15);
                block[2] = _mm_aesenc_si128(block[2], b);
            }

            // indirectly mix hashing lanes
            const __m128i mix =
                _mm_xor_si128(_mm_xor_si128(block[0], block[1]), _mm_xor_si128(block[2], block[3]));
            block[0] = _mm_aesenc_si128(block[0], mix);
            block[1] = _mm_aesenc_si128(block[1], mix);
            block[2] = _mm_aesenc_si128(block[2], mix);
            block[3] = _mm_aesenc_si128(block[3], mix);

            // reduction from 512-bit block size to 128-bit hash
            __m128i hash = _mm_aesenc_si128(_mm_aesenc_si128(block[0], block[1]),
                                            _mm_aesenc_si128(block[2], block[3]));

            // this algorithm construction requires no less than 1 round to finalize
            input_bytes = FINALIZED;
            return _mm_aesenc_si128(hash, _mm_set_epi64x(CONSTANT_64_9, CONSTANT_64_10));
        }
    }
};
