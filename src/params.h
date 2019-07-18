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
#include <cstdio>

namespace aquahash {
    struct Params {
        enum PARAMS : uint32_t {
            NONE = 0,
            VERBOSE = 1,
            COLOR = 1 << 1,
            AQUAHASH = 1 << 2,
            XXHASH = 1 << 3,
            USE_BIG_ENDIAN = 1 << 4,
        };
        static bool verbose(const int flags) { return (flags & VERBOSE) > 0; }
        static bool color(const int flags) { return (flags & COLOR) > 0; }
        static bool use_aquahash(const int flags) { return (flags & AQUAHASH) > 0; }
        static bool use_xxhash(const int flags) { return (flags & XXHASH) > 0; }
        static bool big_endian(const int flags) { return (flags & USE_BIG_ENDIAN) > 0; }
        static void print(const int flags) {
            printf("verbose: %s\n", verbose(flags) ? "yes" : "no");
            printf("color: %s\n", color(flags) ? "yes" : "no");
            printf("little-endian: %s\n", big_endian(flags) ? "yes" : "no");
            printf("use_aquahash: %s\n", use_aquahash(flags) ? "yes" : "no");
            printf("use_xxhash: %s\n", use_xxhash(flags) ? "yes" : "no");
        }
    };
} // namespace aquahash
