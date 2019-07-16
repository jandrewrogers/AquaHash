#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "aquahash.h"
#include "doctest/doctest.h"
#include "fmt/format.h"
#include <string>
#include <tuple>
#include <vector>

TEST_CASE("Integral") {
    const __m128i seed = _mm_setzero_si128();
    std::vector<int> x{1, 2, 3, 4, 5, 6, 7, 8};
    using T = decltype(x)::value_type;
    auto result = AquaHash::Hash((uint8_t *)(&x), sizeof(T), seed);
    std::size_t hashcode;
    memcpy(&hashcode, &result, sizeof(std::size_t));
}

TEST_CASE("Floating point") {}

TEST_CASE("String") {}

TEST_CASE("Array") {}
