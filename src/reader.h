#pragma once
#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

namespace aquahash {
    // A reader class which reads data in blocks.
    constexpr size_t READ_TRUNK_SIZE = 1 << 17; // This is an optimum trunk size.
    template <typename Policy, size_t BUFFER_SIZE = READ_TRUNK_SIZE> struct FileReader : public Policy {
        template <typename... Args> FileReader(Args... args) : Policy(std::forward<Args>(args)...) {}

        char read_buffer[BUFFER_SIZE];

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
            const size_t block_count = (buf.st_size / BUFFER_SIZE) + (buf.st_size % BUFFER_SIZE != 0);
            for (size_t blk = 0; blk < block_count; ++blk) {
                long nbytes = ::read(fd, read_buffer, BUFFER_SIZE);
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
