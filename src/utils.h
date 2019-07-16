#pragma once

#include <array>
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

    struct Writer {};
} // namespace aquahash
