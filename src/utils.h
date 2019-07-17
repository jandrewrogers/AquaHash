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

#include <array>
#include <cstdint>
#include <cstdio>
#include <immintrin.h>
#include <random>
#include <string>

namespace aquahash {
    class CharGenerator {
      public:
        std::string operator()(const size_t len) {
            std::string str(len, 0);
            for (size_t idx = 0; idx < len; ++idx) {
                str[idx] = valid_characters[rgn() % N];
            }
            return str;
        }

      private:
        std::mt19937 rgn;
        static constexpr int N = 62;
        const std::array<char, N> valid_characters = {
            {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
             'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F',
             'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
             'W', 'X', 'Y', 'Z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'}};
    };

    class HashCodeWriter {
      private:
        char buffer[32];

      public:
        const char *operator()(__m128i value) {
            alignas(16) uint8_t v[16];
            _mm_store_si128((__m128i *)v, value);
            sprintf(buffer, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x", v[0], v[1],
                    v[2], v[3], v[4], v[5], v[6], v[7], v[8], v[9], v[10], v[11], v[12], v[13], v[14],
                    v[15]);
            return buffer;
        }
    };
} // namespace aquahash
