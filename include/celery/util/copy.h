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
    /**
     * @file
     * @brief Utility copy helpers.
     *
     * This header provides a small, optimized `Copy` template that
     * copies a contiguous range of elements from `src` to `dst`.
     *
     * Behavior:
     * - If `T` is trivially constructible the implementation uses
     *   `std::memcpy` for a fast, byte-wise copy of `count * sizeof(T)` bytes.
     * - Otherwise the implementation constructs each element in-place
     *   at `dst` using the copy constructor (`new (&dst[i]) T(src[i]);`).
     *
     * Preconditions:
     * - The destination memory at `dst` must be valid and large enough to hold
     *   `count` elements of type `T`. The caller is responsible for ensuring
     *   capacity (e.g., via `EnsureGrowth` or `Resize` in the container code).
     * - `src` and `dst` may overlap only if such overlap is explicitly handled
     *   by the caller (this function does not perform overlap-safe copying).
     */

    /**
     * @brief Copy `count` elements of type `T` from `src` to `dst`.
     *
     * This function optimizes the copy depending on the properties of `T`.
     * For trivially constructible types a single `std::memcpy` is performed.
     * For non-trivial types each element is constructed in place at the
     * destination using the copy constructor.
     *
     * @tparam T Element type being copied.
     * @param src Pointer to the source array of `T`. Must point to at least `count` elements.
     * @param dst Pointer to the destination array of `T`. Must point to at least `count` elements
     *            and be suitably aligned for placement-new of `T`.
     * @param count Number of elements to copy. If zero, the function returns immediately.
     *
     * @note The function assumes the destination storage is already allocated and
     *       ready for construction (i.e., the caller has ensured capacity).
     * @note This function does not attempt to destroy existing objects at `dst`.
     * @note For trivially copyable types this is equivalent to copying bytes and is very fast.
     */
    template <typename T>
    void Copy(const T *src, T *dst, const size_t count)
    {
        if (count == 0) return; // Nothing to copy

        // This method assumes either
        // EnsureGrowth or Resize has been called already.
        if constexpr (std::is_trivially_constructible_v<T>)
        {
            // Use memcpy directly for trivially constructible types.
            std::memcpy(
                dst,
                src,
                count * sizeof(T)
            );
        }
        else
        {
            // Copy each element by invoking the copy constructor in-place.
            for (Trait::VeryLarge i = 0; i < count; ++i)
            {
                // Use placement-new with the copy constructor.
                new (&dst[i]) T(src[i]);
            }
        }
    }
};
