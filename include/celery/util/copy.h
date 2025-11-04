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
#include "celery/trait/default.h"
#include <cstring>
#include <type_traits>

namespace Celery::Utility
{
    template <typename T>
    void Copy(const T *src, T *dst, const size_t count)
    {
        if (count == 0) return; // Nothing to copy

        // This method assumes either
        // EnsureGrowth or Resize has been called already.
        if constexpr (std::is_trivially_constructible_v<T>)
        {
            // Use memcpy directly
            std::memcpy(
                dst,
                src,
                count * sizeof(T)
            );
        }
        else
        {
            // Copy each element
            for (Trait::VeryLarge i = 0; i < count; ++i)
            {
                // Use copy constructor
                new (&dst[i]) T(src[i]);
            }
        }
    }
};