#pragma once

#include <aquahash.h>
#include <stdio.h>
#include <unistd.h>
#include <utils.h>

namespace aquahash {
    class ComputeHashPolicy {
      public:
        static constexpr size_t BUFFER_SIZE = 1 << 16;
        ComputeHashPolicy() : count(0), seed(_mm_setzero_si128()), hashcode(seed), aqua(seed), writer() {}

        /* Update the hash code */
        void process(const char *buffer, const size_t len) {
            ++count;
            if ((len < BUFFER_SIZE) && (count == 1)) {
                hashcode = AquaHash::Hash(reinterpret_cast<const uint8_t *>(buffer), len, seed);
                return;
            }
            aqua.Update(reinterpret_cast<const uint8_t *>(buffer), len);
        }

        /* Finalize the process and return the hash string. */
        void finalize(const std::string &filename) {
            if (count > 1) hashcode = aqua.Finalize();

            printf("AquaHash(%s) = %s\n", filename.data(), writer(hashcode));
        }

      private:
        size_t count;
        __m128i seed;
        __m128i hashcode;
        AquaHash aqua;
        HashCodeWriter writer;
    };
} // namespace aquahash
