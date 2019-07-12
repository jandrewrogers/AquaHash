#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "aquahash.h"
#include "doctest/doctest.h"
#include "fmt/format.h"
#include <string>
#include <tuple>
#include <vector>
#include <x86intrin.h>

template <typename T> bool test_pod_type(T x, T y) {
    const __m128i seed = _mm_setzero_si128();
    return _mm_test_all_ones(
        _mm_cmpeq_epi8(AquaHash::Hash((uint8_t *)(&x), sizeof(int), seed), AquaHash::Hash((uint8_t *)(&y), sizeof(int), seed)));
}

bool test_string_type(const std::string &first, const std::string &second) {
    const __m128i seed = _mm_setzero_si128();
    return _mm_test_all_ones(_mm_cmpeq_epi8(AquaHash::Hash((uint8_t *)(first.data()), first.size(), seed),
                                            AquaHash::Hash((uint8_t *)(second.data()), second.size(), seed)));
}

template <typename T> bool test_array_type(const std::vector<T> &first, const std::vector<T> &second) {
    const __m128i seed = _mm_setzero_si128();
    return _mm_test_all_ones(_mm_cmpeq_epi8(AquaHash::Hash((uint8_t *)(first.data()), first.size() * sizeof(T), seed),
                                            AquaHash::Hash((uint8_t *)(second.data()), second.size() * sizeof(T), seed)));
}

TEST_CASE("POD") {
    SUBCASE("int") {
        int x = 17;
        int y = 18;
        CHECK(!test_pod_type(x, y));
        CHECK(test_pod_type(x, x));
        CHECK(test_pod_type(y, y));
    }

    SUBCASE("float") {
        float x = 128.3;
        float y = 17823.12345;
        CHECK(!test_pod_type(x, y));
        CHECK(test_pod_type(x, x));
        CHECK(test_pod_type(y, y));
    }

    SUBCASE("double") {
        double x = 128.3;
        double y = 117823.12345;
        CHECK(!test_pod_type(x, y));
        CHECK(test_pod_type(x, x));
        CHECK(test_pod_type(y, y));
    }
}

TEST_CASE("String") {
    CHECK(!test_string_type("Hello", "hello"));
    CHECK(test_string_type("Hello", "Hello"));
}

TEST_CASE("Array") {
    std::vector<int> x{1, 2, 3, 4, 5, 6, 7, 8};
    std::vector<int> y{1, 2, 3, 5, 5, 6, 7, 8};
    CHECK(!test_array_type(x, y));
    CHECK(test_array_type(x, x));
    CHECK(test_array_type(y, y));
}
