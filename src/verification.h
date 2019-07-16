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

#include "aquahash.h"
#include <cassert>
#include <cstring>

namespace aquahash {
    // Verifies the implementation matches test vectors computed several ways.
    // Returns zero on success or line number on test failure.
    static int VerifyImplementation() {
        // A 31-byte string is the smallest key that will exercise all hash
        // computation branches in the small key algorithm
        static constexpr char test_key_small[] = "0123456789012345678901234567890";
        assert(strlen(test_key_small) == 31);

        // A 127-byte string is the smallest key that will exercise all hash
        // computation branches in the large key algorithm
        static constexpr char test_key_large[] = "01234567890123456789012345678901"
                                                 "23456789012345678901234567890123"
                                                 "45678901234567890123456789012345"
                                                 "6789012345678901234567890123456";
        assert(strlen(test_key_large) == 127);

        // TEST INITIALIZERS

        const __m128i initialize_0 = _mm_setzero_si128();
        const __m128i initialize_1 = _mm_set1_epi64x(std::numeric_limits<uint64_t>::max());

        // TEST VECTOR HASHES

        // Hash(test_key_small, 31, initialize_0)
        const uint8_t valid_31_0[] = {0x4E, 0xF7, 0x44, 0xCA, 0xC8, 0x10, 0xCB, 0x77,
                                      0x90, 0xD7, 0x9E, 0xDB, 0x0E, 0x6E, 0xBE, 0x9B};
        // Hash(test_key_small, 31, initialize_1)
        const uint8_t valid_31_1[] = {0x30, 0xE9, 0xEF, 0xE4, 0x6B, 0x5C, 0x05, 0x2E,
                                      0xED, 0x62, 0xE3, 0xA4, 0x90, 0x77, 0x46, 0x01};

        // Hash(test_key_large, 127, initialize_0)
        const uint8_t valid_127_0[] = {0x7A, 0x39, 0xDA, 0xDC, 0x21, 0x50, 0xFB, 0xF2,
                                       0x78, 0x92, 0xC1, 0x1C, 0x25, 0xAA, 0x03, 0x4E};
        // Hash(test_key_large, 127, initialize_1)
        const uint8_t valid_127_1[] = {0x0E, 0xDD, 0x5A, 0x3A, 0xB7, 0x4B, 0xFA, 0xC3,
                                       0xFF, 0x73, 0x84, 0xA2, 0x8B, 0xB9, 0xBF, 0x13};

        // small key algorithm test with first initializer
        {
            auto hash = AquaHash::SmallKeyAlgorithm(reinterpret_cast<const uint8_t *>(test_key_small),
                                                    strlen(test_key_small), initialize_0);
            if (memcmp(&hash, &valid_31_0, sizeof(hash))) return __LINE__;
        }

        // small key algorithm test with second initializer
        {
            auto hash = AquaHash::SmallKeyAlgorithm(reinterpret_cast<const uint8_t *>(test_key_small),
                                                    strlen(test_key_small), initialize_1);
            if (memcmp(&hash, &valid_31_1, sizeof(hash))) return __LINE__;
        }

        // large key algorithm test with first initializer
        {
            auto hash = AquaHash::LargeKeyAlgorithm(reinterpret_cast<const uint8_t *>(test_key_large),
                                                    strlen(test_key_large), initialize_0);
            if (memcmp(&hash, &valid_127_0, sizeof(hash))) return __LINE__;
        }

        // large key algorithm test with second initializer
        {
            auto hash = AquaHash::LargeKeyAlgorithm(reinterpret_cast<const uint8_t *>(test_key_large),
                                                    strlen(test_key_large), initialize_1);
            if (memcmp(&hash, &valid_127_1, sizeof(hash))) return __LINE__;
        }

        // make sure hybrid algorithm matches underlying algorithm components
        {
            __m128i hash;

            hash = AquaHash::Hash(reinterpret_cast<const uint8_t *>(test_key_small), strlen(test_key_small),
                                  initialize_0);
            if (memcmp(&hash, &valid_31_0, sizeof(hash))) return __LINE__;

            hash = AquaHash::Hash(reinterpret_cast<const uint8_t *>(test_key_small), strlen(test_key_small),
                                  initialize_1);
            if (memcmp(&hash, &valid_31_1, sizeof(hash))) return __LINE__;

            hash = AquaHash::Hash(reinterpret_cast<const uint8_t *>(test_key_large), strlen(test_key_large),
                                  initialize_0);
            if (memcmp(&hash, &valid_127_0, sizeof(hash))) return __LINE__;

            hash = AquaHash::Hash(reinterpret_cast<const uint8_t *>(test_key_large), strlen(test_key_large),
                                  initialize_1);
            if (memcmp(&hash, &valid_127_1, sizeof(hash))) return __LINE__;
        }

        // verify incremental algorithm against non-incremental algorithms
        {
            // incremental on small key one-shot
            {
                AquaHash aqua(initialize_0);
                aqua.Update(reinterpret_cast<const uint8_t *>(test_key_small), strlen(test_key_small));
                auto hash = aqua.Finalize();
                if (memcmp(&hash, &valid_31_0, sizeof(hash))) return __LINE__;
            }

            // incremental on large key one-shot
            {
                AquaHash aqua(initialize_0);
                aqua.Update(reinterpret_cast<const uint8_t *>(test_key_large), strlen(test_key_large));
                auto hash = aqua.Finalize();
                if (memcmp(&hash, &valid_127_0, sizeof(hash))) return __LINE__;
            }

            // incremental using every possible chunk size across block boundary
            {
                for (size_t span = 1; span <= strlen(test_key_large); span++) {
                    AquaHash aqua(initialize_0);
                    size_t div = strlen(test_key_large) / span;
                    size_t mod = strlen(test_key_large) % span;
                    for (size_t j = 0; j < div; j++) {
                        aqua.Update(reinterpret_cast<const uint8_t *>(&test_key_large[j * span]), span);
                    }
                    aqua.Update(
                        reinterpret_cast<const uint8_t *>(&test_key_large[strlen(test_key_large) - mod]),
                        mod);
                    auto hash = aqua.Finalize();
                    if (memcmp(&hash, &valid_127_0, sizeof(hash))) return __LINE__;
                }
            }
        }

        return 0;
    }
} // namespace aquahash
