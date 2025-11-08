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
#include "celery/trait/type.h"
#include "xxhash.h"
#include <concepts>

namespace Celery::Base
{
    /**
     * @brief CRTP mixin that provides hashing functionality.
     *
     * This class defines a static method `Get` that computes
     * a hash value for an object of type T. By default, the
     * method is deleted and must be specialized for specific types.
     *
     * @tparam T The type to provide hashing for.
     */
    template<typename T>
    struct Hash
    {
        /**
         * @brief Compute the hash value for the given object.
         *
         * This method must be specialized for specific types
         * to provide a valid hash computation.
         *
         * @param obj The object to hash.
         * @return Trait::Uint The computed hash value.
         */
        template<
            class U,
            typename = Trait::EnsureSame<U, T>
        >
        static inline Trait::Uint Get(U &&obj)
        {
            // Default implementation uses XXH3 hash on the raw bytes of the object
            auto hash = XXH3_64bits(&obj, sizeof(Trait::GetBase<U>));
            return static_cast<Trait::Uint>(hash);
        }
    };

    /**
     * @brief Concept to check if a type T is hashable.
     *
     * This concept requires that the type T has a valid
     * specialization of Base::Hash that provides a `Get`
     * method returning a Trait::Uint.
     *
     * @tparam T The type to check for hashability.
     */
    template<typename T>
    concept Hashable = requires(T &&obj)
    {
        { Hash<T>::Get(std::forward<T>(obj)) } ->
            std::convertible_to<Trait::Uint>;
    };
}