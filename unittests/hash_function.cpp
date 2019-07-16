#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "aquahash.h"
#include "doctest/doctest.h"
#include "fmt/format.h"
#include "interface.h"
#include "utils.h"
#include <string>
#include <tuple>
#include <vector>
#include <x86intrin.h>

template <typename T> bool test_pod_type(T x, T y) {
    const __m128i seed = _mm_setzero_si128();
    return _mm_test_all_ones(_mm_cmpeq_epi8(AquaHash::Hash((uint8_t *)(&x), sizeof(T), seed),
                                            AquaHash::Hash((uint8_t *)(&y), sizeof(T), seed)));
}

bool test_string_type(const std::string &first, const std::string &second) {
    const __m128i seed = _mm_setzero_si128();
    return _mm_test_all_ones(
        _mm_cmpeq_epi8(AquaHash::Hash((uint8_t *)(first.data()), first.size(), seed),
                       AquaHash::Hash((uint8_t *)(second.data()), second.size(), seed)));
}

template <typename T> bool test_array_type(const std::vector<T> &first, const std::vector<T> &second) {
    const __m128i seed = _mm_setzero_si128();
    return _mm_test_all_ones(
        _mm_cmpeq_epi8(AquaHash::Hash((uint8_t *)(first.data()), first.size() * sizeof(T), seed),
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

TEST_CASE("Basic tests") {
    std::vector<int> x{1, 2, 3, 4, 5, 6, 7, 8};
    std::vector<int> y{1, 2, 3, 5, 5, 6, 7, 8};

    SUBCASE("Basic tests") {
        aquahash::Hash h;

        // Compute the hash code for the whole vector
        std::size_t expected_result = 172989338662585445l;
        CHECK(h(x) != h(y));
        CHECK(h(x) == expected_result);
        CHECK(h(x.data(), x.size()) == expected_result);
        CHECK(h(&x[0], x.size()) == expected_result);

        // Compute the hash code for part of the vector
        std::size_t new_hash_code = 4302930102154141567l;
        CHECK(h(x.data(), x.size() - 1) == new_hash_code);
    }

    static constexpr char test_key_small[] = "0123456789012345678901234567890";
    static constexpr char test_key_large[] = "01234567890123456789012345678901"
                                             "23456789012345678901234567890123"
                                             "45678901234567890123456789012345"
                                             "6789012345678901234567890123456";
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

    SUBCASE("SmallKeyAlgorithm") {
        SUBCASE("First init value") {
            auto hash = AquaHash::SmallKeyAlgorithm(reinterpret_cast<const uint8_t *>(test_key_small),
                                                    strlen(test_key_small), initialize_0);
            CHECK(memcmp(&hash, &valid_31_0, sizeof(hash)) == 0);
        }

        SUBCASE("Second init value") {
            auto hash = AquaHash::SmallKeyAlgorithm(reinterpret_cast<const uint8_t *>(test_key_small),
                                                    strlen(test_key_small), initialize_1);
            CHECK(memcmp(&hash, &valid_31_1, sizeof(hash)) == 0);
        }
    }

    SUBCASE("LargeKeyAlgorithm") {
        SUBCASE("First init value") {
            auto hash = AquaHash::LargeKeyAlgorithm(reinterpret_cast<const uint8_t *>(test_key_large),
                                                    strlen(test_key_large), initialize_0);
            CHECK(memcmp(&hash, &valid_127_0, sizeof(hash)) == 0);
        }

        // large key algorithm test with second initializer
        SUBCASE("Second init value") {
            auto hash = AquaHash::LargeKeyAlgorithm(reinterpret_cast<const uint8_t *>(test_key_large),
                                                    strlen(test_key_large), initialize_1);
            CHECK(memcmp(&hash, &valid_127_1, sizeof(hash)) == 0);
        }
    }

    SUBCASE("Hybrid algorithm") {
        __m128i hash;

        hash = AquaHash::Hash(reinterpret_cast<const uint8_t *>(test_key_small), strlen(test_key_small),
                              initialize_0);
        CHECK(memcmp(&hash, &valid_31_0, sizeof(hash)) == 0);

        hash = AquaHash::Hash(reinterpret_cast<const uint8_t *>(test_key_small), strlen(test_key_small),
                              initialize_1);
        CHECK(memcmp(&hash, &valid_31_1, sizeof(hash)) == 0);

        hash = AquaHash::Hash(reinterpret_cast<const uint8_t *>(test_key_large), strlen(test_key_large),
                              initialize_0);
        CHECK(memcmp(&hash, &valid_127_0, sizeof(hash)) == 0);

        hash = AquaHash::Hash(reinterpret_cast<const uint8_t *>(test_key_large), strlen(test_key_large),
                              initialize_1);
        CHECK(memcmp(&hash, &valid_127_1, sizeof(hash)) == 0);
    }

    SUBCASE("Incremental algorithm") {
        SUBCASE("incremental on small key one-shot") {
            AquaHash aqua(initialize_0);
            aqua.Update(reinterpret_cast<const uint8_t *>(test_key_small), strlen(test_key_small));
            auto hash = aqua.Finalize();
            CHECK(memcmp(&hash, &valid_31_0, sizeof(hash)) == 0);
        }

        SUBCASE("incremental on large key one-shot") {
            AquaHash aqua(initialize_0);
            aqua.Update(reinterpret_cast<const uint8_t *>(test_key_large), strlen(test_key_large));
            auto hash = aqua.Finalize();
            CHECK(memcmp(&hash, &valid_127_0, sizeof(hash)) == 0);
        }

        SUBCASE("incremental using every possible chunk size across block boundary") {
            for (size_t span = 1; span <= strlen(test_key_large); span++) {
                AquaHash aqua(initialize_0);
                size_t div = strlen(test_key_large) / span;
                size_t mod = strlen(test_key_large) % span;
                for (size_t j = 0; j < div; j++) {
                    aqua.Update(reinterpret_cast<const uint8_t *>(&test_key_large[j * span]), span);
                }
                aqua.Update(
                    reinterpret_cast<const uint8_t *>(&test_key_large[strlen(test_key_large) - mod]), mod);
                auto hash = aqua.Finalize();
                CHECK(memcmp(&hash, &valid_127_0, sizeof(hash)) == 0);
            }
        }
    }
}
