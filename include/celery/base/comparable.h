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
#include <concepts>

namespace Celery::Base
{
    /**
     * @brief Primary template for Compare struct.
     *
     * Specializations of this struct should provide a callable
     * operator() that takes an instance of T and returns a size_t
     * representing the comparison result.
     *
     * @tparam T The type to be compared.
     */
    template <typename T>
    struct Compare;

    /**
     * @brief Concept to check if a type T has a valid Compare specialization.
     *
     * A type T satisfies the Comparable concept if there exists a
     * Compare<T> specialization that can be invoked with an instance
     * of T and returns a size_t convertible result.
     *
     * @tparam T The type to be checked for comparability.
     */
    template <typename T>
    concept Comparable = requires(T&& t) {
            { Compare<std::remove_cvref_t<T>>{}(std::forward<T>(t)) }
            -> std::convertible_to<std::size_t>;
    };
}