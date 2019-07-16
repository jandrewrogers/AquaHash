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
#include "utils.h"

namespace aquahash {
    struct Hash {
        using result_type = std::size_t;
        const __m128i kSeed = _mm_setzero_si128();

        template <typename T> T convert(const __m128i h) const noexcept {
            T result;
            memcpy(&result, &h, sizeof(T));
            return result;
        }

        template <typename T> result_type operator()(const T value) const noexcept {
            return convert<std::size_t>(AquaHash::Hash((uint8_t *)(&value), sizeof(T), kSeed));
        }

        template <typename T> result_type operator()(const T *ptr, const size_t len) const noexcept {
            return convert<std::size_t>(AquaHash::Hash((uint8_t *)(ptr), len * sizeof(T), kSeed));
        }

        template <typename T> result_type operator()(const std::string &key) const noexcept {
            return convert<std::size_t>(AquaHash::Hash((uint8_t *)(key.data()), key.size(), kSeed));
        }

        template <typename T> result_type operator()(const std::vector<T> &data) const noexcept {
            return convert<std::size_t>(
                AquaHash::Hash((uint8_t *)(data.data()), data.size() * sizeof(T), kSeed));
        }
    };
} // namespace aquahash
