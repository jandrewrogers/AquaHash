#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "aquahash.h"
#include "doctest/doctest.h"
#include "fmt/format.h"
#include "interface.h"
#include "utils.h"
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

TEST_CASE("String") {
    std::unordered_set<std::string, aquahash::hash<std::string>, std::equal_to<std::string>> lookup;
    aquahash::CharGenerator gen;
    constexpr int N = 1024;
    for (int idx = 0; idx < N; ++idx) {
        auto const item = gen(32);
        lookup.emplace(item);
        lookup.emplace(item);
        lookup.emplace(item);
    }
    CHECK(lookup.size() == N); // It is almost impossible to fail this test point.
    // for (auto item : lookup) fmt::print("{}\n", item);
}

TEST_CASE("String") {
    std::unordered_set<std::string, aquahash::hash<std::string>> lookup;
    aquahash::CharGenerator gen;
    constexpr int N = 1024;
    for (int idx = 0; idx < N; ++idx) {
        auto const item = gen(32);
        lookup.emplace(item);
        lookup.emplace(item);
        lookup.emplace(item);
    }
    CHECK(lookup.size() == N); // It is almost impossible to fail this test point.
    // for (auto item : lookup) fmt::print("{}\n", item);
}

TEST_CASE("vector") {
    std::unordered_set<std::vector<int>, aquahash::hash<std::vector<int>>> lookup;
    lookup.emplace(std::vector<int>{1,2,3,4,5});
    lookup.emplace(std::vector<int>{1,2,3,4,5});
    lookup.emplace(std::vector<int>{1,2,3,4,5});
    CHECK(lookup.size() == 1); // It is almost impossible to fail this test point.
}
