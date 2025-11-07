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
    using GetBase = std::decay_t<T>;

    /**
     * @brief Compile-time boolean to check if two types share the same base type.
     *
     * This variable template evaluates to true if the base types
     * of T and U (after removing references and const qualifiers)
     * are the same; otherwise, it evaluates to false.
     *
     * @tparam T First type to compare.
     * @tparam U Second type to compare.
     */
    template <typename T, typename U>
    constexpr bool EnsureSameBase = std::is_same_v<
        GetBase<T>,
        GetBase<U>
    >;

    /**
     * @brief SFINAE helper to enable functions only if two types share the same base type.
     *
     * This alias template resolves to `std::enable_if` when
     * `EnsureSameBase<T, U>` is true, allowing function templates
     * to be conditionally enabled based on type compatibility.
     *
     * @tparam T First type to compare.
     * @tparam U Second type to compare.
     */
    template <typename T, typename U>
    using EnsureSame = std::enable_if<EnsureSameBase<T, U>>;
}