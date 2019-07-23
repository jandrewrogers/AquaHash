#pragma once

#include "utils.h"

namespace {
    std::string generate_random_string() {
        const char *var = std::getenv("LEN");
        if (var == nullptr) {
            throw std::runtime_error("LEN environment variable is required!");
        }
        int len = std::stoi(var);
        std::cout << "String length: " << len << "\n";
        aquahash::CharGenerator gen;
        return gen(len);
    }
} // namespace
