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

/**
 * @brief Lightweight base template that provides a raw buffer pointer for derived types.
 *
 * This header declares the Celery::Base::Bufferable template which exposes a protected
 * raw pointer to a contiguous buffer of elements of type `T`. The template itself
 * does not perform allocation or deallocation; memory management is the responsibility
 * of derived classes or external owners.
 *
 * Typical uses:
 * - Provide a common member for container-like types that manage an underlying buffer.
 * - Allow derived types to implement custom allocation strategies while sharing a
 *   consistent member name and access level.
 */

namespace Celery::Base
{
    /**
     * @brief CRTP-style or simple base for types that hold a raw buffer pointer.
     *
     * @tparam T Element type stored in the buffer.
     *
     * Notes:
     * - `Bufferable` does not define constructors, destructors, or copy/move semantics.
     *   Derived classes must implement appropriate resource management if they own the buffer.
     * - The `buffer` pointer is `protected` so derived classes can access it directly.
     * - The pointer is initialized to `nullptr` to indicate an empty/unallocated buffer.
     */
    template <typename T>
    class Bufferable
    {
    protected:
        /**
         * @brief Pointer to a contiguous array of `T` elements.
         *
         * - When `nullptr`, no buffer is associated.
         * - If non-null, points to memory that is expected to hold elements of type `T`.
         * - Ownership semantics are not defined by this base class. Derived classes
         *   must clearly document whether they own and must free the memory or if it
         *   is managed externally.
         */
        T *data = nullptr;

    public:
        /**
         * @brief Accessor for the raw buffer pointer.
         *
         * @return Pointer to the underlying buffer of type `T`.
         */
        T *Ptr()
        const {
            return data;
        }
    };
}
