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
#include "Celery/trait/default.h"
#include <xxhash.h>

namespace Celery::Misc
{
    /**
     * @brief A hashing utility that provides a generic hash function.
     *
     * This struct uses the XXH3_64bits hashing algorithm to compute
     * a hash value for a given object. The resulting hash is cast
     * to the `Trait::VeryLarge` type.
     */
    struct Hash
    {
        /// @brief Marker type indicating that the hash function is avalanching.
        using is_avalanching = void;

        /**
         * @brief Computes the hash value for the given object.
         *
         * @tparam T The type of the object to hash.
         * @param obj The object to hash. It is forwarded as an rvalue reference.
         * @return Trait::VeryLarge The computed hash value.
         *
         * @note This function is marked noexcept, ensuring it does not throw exceptions.
         */
        template <typename T>
        Trait::VeryLarge operator()(T &&obj) const noexcept
        {
            auto hash = XXH3_64bits(&obj, sizeof(T));
            return static_cast<Trait::VeryLarge>(hash);
        }
    };
}
