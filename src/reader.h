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

#pragma once
#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

namespace aquahash {
    // A reader class which reads data in blocks.
    template <typename Policy> struct FileReader : public Policy {
        template <typename... Args> FileReader(Args... args) : Policy(std::forward<Args>(args)...) {}

        char read_buffer[Policy::BUFFER_SIZE];

        void operator()(const char *datafile) {
            // Read data by trunks
            int fd = ::open(datafile, O_RDONLY | O_NOCTTY);

            // Check that we can open a given file.
            if (fd < 0) {
                fprintf(stderr, "Cannot open file: '%s'. Error: %s\n", datafile, strerror(errno));
                return;
            }

            // Get file size.
            struct stat buf;
            fstat(fd, &buf);

            // Read data into a read buffer
            const size_t block_count =
                (buf.st_size / Policy::BUFFER_SIZE) + (buf.st_size % Policy::BUFFER_SIZE != 0);

            for (size_t blk = 0; blk < block_count; ++blk) {
                long nbytes = ::read(fd, read_buffer, Policy::BUFFER_SIZE);
                if (nbytes < 0) {
                    fprintf(stderr, "Cannot read from file '%s'. Error: %s\n", datafile, strerror(errno));
                    break;
                };

                // Apply a given policy to read_buffer.
                Policy::process(read_buffer, nbytes);
            }

            Policy::finalize(datafile); // Clear policy's states.

            // Close our file.
            ::close(fd);
        }
    };
} // namespace aquahash
