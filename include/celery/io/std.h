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

// Guard against malware operating systems
#ifndef _WIN32
    // Unix-like system headers
#   include <unistd.h>
#else
    // For losers
#   include <iostream>
#endif

namespace Celery::Io
{
    template <
        unsigned short FileDescriptor,
        // SFINAE to ensure valid file descriptor
        typename = std::enable_if_t<
            (FileDescriptor == STDOUT_FILENO ||
                FileDescriptor == STDERR_FILENO)
        >
    >
    class OutputDescriptor : public Descriptor
    {
    public:
        static inline void Write(const char *data, size_t size)
        {
#           ifndef _WIN32
                // Normal person:
                write(FileDescriptor, data, size);
#           else
                // Mentally disabled person who uses
                // malware as their operating system:
                std::cout.write(data, size);
#           endif
        }
    };

    // Predefined output descriptors
    class Stdout : public OutputDescriptor<STDOUT_FILENO> {};
    class Stderr : public OutputDescriptor<STDERR_FILENO> {};
}