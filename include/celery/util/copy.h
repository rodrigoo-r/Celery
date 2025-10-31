//
// Created by rodrigo on 31/10/25.
//

#pragma once
#include "celery/trait/default.h"
#include <cstring>
#include <type_traits>

namespace Celery::Utility
{
    template <typename T>
    void Copy(const T *src, T *dst, const size_t count)
    {
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