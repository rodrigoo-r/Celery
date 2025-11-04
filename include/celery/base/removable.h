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

namespace Celery::Base
{
    /**
     * @brief CRTP mixin that provides removal operator overloads forwarding to the derived container.
     *
     * @tparam Derived The final type that inherits from Removable\<Derived, T\>. Must implement:
     *         - void Remove(T&&) or equivalent overloads for removal behavior used by operator-=
     *
     * @tparam T The element type the container holds.
     *
     * @note This class uses static_cast\<Derived*\>(this) to forward calls to the derived type
     *       (CRTP). It does not provide storage or semantic guarantees itself.
     */
    template <typename Derived, typename T>
    class Removable
    {
    public:
        /**
         * @brief Remove an element (or perform a front-removal semantics) by forwarding to Derived::Remove.
         *
         * This operator uses SFINAE to constrain U so that its decayed type matches the decayed T.
         * That avoids accidental overload resolution with incompatible types.
         *
         * @tparam U Type of the supplied value (deduced). Defaults to T.
         * @param value The value to remove or identify which element to remove (forwarded).
         * @return Reference to the derived container to allow chaining.
         *
         * @note The precise semantics of Remove(...) (e.g., removing the first matching element,
         *       removing by value, or popping front) are determined by the Derived type's implementation.
         */
        template <
            class U = T,
            // SFINAE to ensure U is the same type as T
            typename = std::enable_if_t<
                std::is_same_v<
                    std::decay_t<T>,
                    std::decay_t<U>
                >
            >
        >
        Derived &operator-=(U &&value)
        {
            // Forward to the derived class's EmplaceFront method
            static_cast<Derived*>(this)->Remove(std::forward<U>(value));
            return static_cast<Derived&>(*this);
        }

        /**
         * @brief Remove an element (or perform a front-removal semantics) by forwarding to Derived::Remove.
         *
         * This method uses SFINAE to constrain U so that its decayed type matches the decayed T.
         * That avoids accidental overload resolution with incompatible types.
         *
         * @tparam U Type of the supplied value (deduced). Defaults to T.
         * @param value The value to remove or identify which element to remove (forwarded).
         *
         * @note The precise semantics of Remove(...) (e.g., removing the first matching element,
         *       removing by value, or popping front) are determined by the Derived type's implementation.
         */
        template <
            class U = T,
            // SFINAE to ensure U is the same type as T
            typename = std::enable_if_t<
                std::is_same_v<
                    std::decay_t<T>,
                    std::decay_t<U>
                >
            >
        >
        void Erase(U &&value)
        {
            static_cast<Derived*>(this)->Remove(std::forward<U>(value));
        }
    };
}