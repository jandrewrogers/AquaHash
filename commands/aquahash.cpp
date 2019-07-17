#include "aquahash.h"
#include "clara.hpp"
#include "fmt/format.h"
#include "interface.h"
#include "reader.h"
#include "stream.h"
#include "utils.h"
#include <string>
#include <tuple>
#include <vector>
#include <x86intrin.h>

namespace {
    void disp_version() { printf("%s\n", "aquahash version 1.0"); }
    void copyright() { printf("\n%s\n", "Report bugs or enhancement requests to hungptit@gmail.com"); }
    void usage() {
        printf("\nExamples:\n");
        printf("\nExamples:\n");
    }

    enum PARAMS : uint32_t {
        VERBOSE = 1,
        COLOR = 1 << 1,
        INVERT_MATCH = 1 << 2,
        EXACT_MATCH = 1 << 3,
        IGNORE_CASE = 1 << 4,
        INFO = 1 << 5,
        COUNT = 1 << 6,
    };

    struct InputParams {
        int flags;
        std::vector<std::string> files;

        bool verbose() const { return (flags & VERBOSE) > 0; }

        void print() const {
            fmt::print("File information databases: [\"{}\"]\n", fmt::join(files, "\",\""));
        }
    };

    InputParams parse_input_arguments(int argc, char *argv[]) {
        InputParams params;
        bool verbose = false;
        bool version = false;
        bool help = false;
        std::vector<std::string> dbs;
        std::set<std::string> lookup;

        auto cli = clara::Help(help) |
                   clara::Opt(verbose)["-v"]["--verbose"]("Display verbose information") |
                   clara::Opt(version)["--version"]("Display the version of aquahash command.") |
                   clara::Arg(params.files, "files")("Input files");

        auto result = cli.parse(clara::Args(argc, argv));
        if (!result) {
            fmt::print(stderr, "Invalid option: {}\n", result.errorMessage());
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
            fmt::print("{}", oss.str());
            usage();
            copyright();
            exit(EXIT_SUCCESS);
        }

        // Display input arguments in JSON format if verbose flag is on
        if (params.verbose()) {
            params.print();
        }

        return params;
    }

    void compute(const InputParams &params) {
        for (auto &file : params.files) {
            aquahash::FileReader<aquahash::ComputeHashPolicy> hasher;
            hasher(file.data());
        };
    }
} // namespace

int main(int argc, char *argv[]) {
    auto params = parse_input_arguments(argc, argv);
    compute(params);
}
