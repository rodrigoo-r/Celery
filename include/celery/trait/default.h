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

namespace Celery::Trait
{
    // --- Type Traits --- //
    typedef size_t VeryLarge;
    typedef unsigned int Large;
    typedef unsigned short Small;
    typedef unsigned char Tiny;
    typedef short Short;
    typedef Large Uint;
    typedef int Int;
    typedef char Byte;
    typedef float Float;
    typedef double Decimal;

    // --- Static Assertions --- //
    static_assert(
        sizeof(VeryLarge) >= 8,
        "VeryLarge must be at least 8 bytes"
    );
    static_assert(
        sizeof(Large) >= 4,
        "Large must be at least 4 bytes"
    );
    static_assert(
        sizeof(Small) >= 2,
        "Small must be at least 2 bytes"
    );
    static_assert(
        sizeof(Tiny) >= 1,
        "Tiny must be at least 1 byte"
    );
    static_assert(
        sizeof(Short) >= 2,
        "Short must be at least 2 bytes"
    );
    static_assert(
        sizeof(Int) >= 4,
        "Int must be at least 4 bytes"
    );
    static_assert(
        sizeof(Uint) >= 4,
        "Uint must be at least 4 bytes"
    );
    static_assert(
        sizeof(Byte) == 1,
        "Byte must be 1 byte"
    );

    // --- Default Traits --- //
    constexpr double GrowthFactor = 1.8;
    constexpr unsigned int InitialCapacity = 25;
}