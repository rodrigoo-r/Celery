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

namespace Celery::Base
{
    /**
     * @brief Mixin providing a stored length and simple accessors.
     *
     * Use this base class for types that need to expose a size/length
     * without reimplementing storage and simple query methods.
     */
    class Sizeable
    {
    protected:
        /// Stored length (number of elements).
        size_t len{};

    public:
        /**
         * @brief Return the current stored size.
         * @return The number of elements represented by this object.
         * @note This function is non-throwing and does not modify the object.
         */
        [[nodiscard]] size_t Size()
        const noexcept {
            return len;
        }

        /**
         * @brief Alias for Size\(\) retained for API compatibility.
         * @return The number of elements represented by this object.
         * @note This function is non-throwing and does not modify the object.
         */
        [[nodiscard]] size_t Len()
        const noexcept {
            return len;
        }
    };
}
