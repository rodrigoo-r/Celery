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
#include <type_traits>

namespace Celery::Trait
{
    /**
     * @brief Helper type trait to get the base type by removing
     *        reference, const, and applying decay.
     *
     * This alias template applies `std::decay`, `std::remove_const`,
     * and `std::remove_reference` in sequence to yield the
     * underlying base type of T.
     *
     * @tparam T The type to process.
     */
    template <typename T>
    using GetBase = std::remove_reference<
        std::remove_const<
            std::decay<T>
        >
    >;
}