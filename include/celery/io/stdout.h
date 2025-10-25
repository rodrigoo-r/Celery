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
    class Stdout
    {
    public:
        static inline void Write(const char *data, size_t size)
        {
#           ifndef _WIN32
                // Normal person:
                write(STDOUT_FILENO, data, size);
#           else
                // Mentally disabled person who uses
                // malware as their operating system:
                std::cout.write(data, size);
#           endif
        }
    };
}