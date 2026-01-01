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
#include "Celery/Trait/Default.h"

namespace Celery::Io
{
    class WriteDescriptor
    {
    public:
        static inline void Write(char *data, Trait::VeryLarge size)
        {
            // Placeholder implementation
        }
    };

    class ReadDescriptor
    {
    public:
        static inline Trait::SignedVeryLarge Read(char *buffer, Trait::VeryLarge size)
        {
            // Placeholder implementation
            return 0;
        }
    };
}