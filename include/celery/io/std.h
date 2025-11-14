/*
                    --- Celery ---
    Celery is a high-performance C++ STL-like
    library that provides efficient data structures
    and algorithms for modern applications.

    It is distributed under the Apache 2.0 License,
    if you do not have a copy of the license,
    you can find it at:
        http://www.apache.org/licenses/LICENSE-2.0

    Please see the LICENSE file for more information,
    as individuals are to follow the terms of the license
    as part of a binding agreement. Govern yourself
    accordingly.
*/

#pragma once
#include <cstddef>
#include <iostream>

#include "descriptor.h"

// Guard against incompatible platforms
#ifndef _WIN32
    // Unix-like system headers
#   include <unistd.h>
#else
#    include <string>
#    include <iostream>
#endif

namespace Celery::Io
{
    /** @brief Output descriptor for writing data to standard output/error.
     *
     * This class template represents an output descriptor that can write data
     * to either standard output (STDOUT_FILENO) or standard error (STDERR_FILENO).
     * It inherits from WriteDescriptor and provides a static Write method
     * that performs the actual writing operation.
     *
     * @tparam FileDescriptor The file descriptor to write to (STDOUT_FILENO or STDERR_FILENO).
     *
     * @note SFINAE is used to ensure that only valid file descriptors are accepted.
     */
    template <
        unsigned short FileDescriptor,
        // SFINAE to ensure valid file descriptor
        typename = std::enable_if_t<
            (FileDescriptor == STDOUT_FILENO ||
                FileDescriptor == STDERR_FILENO)
        >
    >
    class OutputDescriptor :
        public WriteDescriptor
    {
    public:
        static inline void Write(char *data, const size_t size)
        {
#           ifndef _WIN32
                // POSIX-compliant write
                write(FileDescriptor, data, size);
#           else
                // Fallback because Windows likes being special:
                std::cout.write(data, size);
#           endif
        }
    };

    /** @brief Input descriptor for reading data from standard input.
     *
     * This class represents an input descriptor that can read data
     * from standard input (STDIN_FILENO). It inherits from ReadDescriptor
     * and provides a static Read method that performs the actual reading operation.
     */
    class InputDescriptor :
        public ReadDescriptor
    {
    public:
        static inline Celery::Str::String Read()
        {
#            ifndef _WIN32
                Celery::Str::String result;
                // POSIX-compliant read from stdin
                while (true)
                {
                    char buffer[1024];
                    ssize_t bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer));
                    if (bytes_read <= 0)
                        break;

                    result.append(buffer, bytes_read);
                }

                return result;
#            else
                // Fallback because Windows likes being special:
                std::string buffer;
                std::getline(std::cin, buffer);
                Celery::Str::String result(buffer.c_str(), buffer.size());
                return result;
#            endif
        }
    };

    // Predefined output descriptors
    class Stdout : public OutputDescriptor<STDOUT_FILENO> {};
    class Stderr : public OutputDescriptor<STDERR_FILENO> {};
    class Stdin  : public InputDescriptor {};
}