#pragma once

#include <aquahash.h>
#include <stdio.h>
#include <unistd.h>
#include <utils.h>

namespace aquahash {
    class ComputeHashPolicy {
      public:
        ComputeHashPolicy() : aqua(_mm_setzero_si128()), writer() {}

        /* Update the hash code */
        void process(const char *buffer, const size_t len) {
            aqua.Update(reinterpret_cast<const uint8_t *>(buffer), len);
        }

        /* Finalize the process and return the hash string. */
        void finalize(const std::string &filename) {
            printf("%s: %s\n", filename.data(), writer(aqua.Finalize()));
        }

      private:
        AquaHash aqua;
        HashCodeWriter writer;
    };
} // namespace aquahash
