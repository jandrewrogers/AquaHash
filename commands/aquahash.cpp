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

#include "aquahash.h"
#include "aquahash_policy.h"
#include "clara.hpp"
#include "interface.h"
#include "params.h"
#include "reader.h"
#include "utils.h"
#include <string>

namespace {
    void disp_version() { printf("%s\n", "aquahash version 1.0"); }
    void copyright() { printf("\n%s\n", "Report bugs or enhancement requests to hungptit@gmail.com"); }
    void usage() {
        printf("\nExamples:\n");
        printf("\taquahash file1 file2 file3:\n");
    }

    void parse_input_arguments(int argc, char *argv[]) {
        bool verbose = false;
        bool version = false;
        bool color = false;
        bool big_endian = false;
        bool use_xxhash = false;
        bool help = false;
        int flags = 0;
        std::vector<std::string> files;
        auto cli = clara::Help(help) |
                   clara::Opt(verbose)["-v"]["--verbose"]("Display verbose information") |
                   clara::Opt(version)["--version"]("Display the version of aquahash command.") |
                   clara::Opt(color)["--color"]("Use color text.") |
                   clara::Opt(use_xxhash)["--use-xxhash"]("Compute checksum using XXHASH64 algorithm.") |
                   clara::Opt(big_endian)["--big-endian"]("Display a hash string using big endian order.") |
                   clara::Arg(files, "files")("Input files");

        auto result = cli.parse(clara::Args(argc, argv));
        if (!result) {
            fprintf(stderr, "Invalid option: %s\n", result.errorMessage().data());
            exit(EXIT_FAILURE);
        }

        if (version) {
            disp_version();
            exit(EXIT_SUCCESS);
        }

        // Print out the help document.
        if (help) {
            std::ostringstream oss;
            oss << cli;
            printf("%s", oss.str().data());
            usage();
            copyright();
            exit(EXIT_SUCCESS);
        }

        flags = (verbose ? aquahash::Params::VERBOSE : aquahash::Params::NONE) |
                (use_xxhash ? aquahash::Params::XXHASH : aquahash::Params::NONE) |
                (color ? aquahash::Params::COLOR : aquahash::Params::NONE);

        // Display input arguments in JSON format if verbose flag is on
        if (aquahash::Params::verbose(flags)) {
            aquahash::Params::print(flags);
        }

        // Compute the hash code
        for (auto const &file : files) {
            aquahash::FileReader<aquahash::AquaHashPolicy> hasher(flags);
            hasher(file.data());
        }
    }
} // namespace

int main(int argc, char *argv[]) { parse_input_arguments(argc, argv); }
