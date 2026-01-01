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
#include <utility>

#include "celery/trait/type.h"

namespace Celery::Base
{
    /**
     * @brief CRTP mixin that provides push/remove operator overloads forwarding to the derived container.
     *
     * @tparam Derived The final type that inherits from Pushable\<Derived, T\>. Must implement:
     *         - void EmplaceBack(T&&) or equivalent overloads for push-back behavior used by operator+=
     *
     * @tparam T The element type the container holds.
     *
     * @note This class uses static_cast\<Derived*\>(this) to forward calls to the derived type
     *       (CRTP). It does not provide storage or semantic guarantees itself.
     */
    template <typename Derived, typename T>
    class Pushable
    {
    public:
        /**
         * @brief Append a value to the container by forwarding to the derived class's EmplaceBack.
         *
         * This operator accepts any argument convertible to T and perfect-forwards it to the
         * Derived::EmplaceBack(...) method. The default template parameter U = T allows
         * usage with lvalues and rvalues.
         *
         * @tparam U Type of the supplied value (deduced). Defaults to T.
         * @param value The value to append (forwarded).
         * @return Reference to the derived container to allow chaining.
         *
         * Example:
         * @code
         * myContainer += value;
         * @endcode
         */
        template <
            class U = T,
            typename = Trait::EnsureSame<T, U>
        >
        Derived &operator+=(U &&value)
        {
            // Forward to the derived class's EmplaceBack method
            static_cast<Derived*>(this)->EmplaceBack(std::forward<U>(value));
            return static_cast<Derived&>(*this);
        }
    };

    /**
     * @brief CRTP mixin that provides a default PushBack implementation using EmplaceBack.
     *
     * This class extends Pushable and provides a default implementation of PushBack
     * that simply forwards to EmplaceBack. This is useful for containers that only
     * implement EmplaceBack and want to support PushBack semantics.
     *
     * @tparam Derived The final type that inherits from DirectlyPushable\<Derived, T\>.
     *         Must implement:
     *         - void EmplaceBack(T&&) or equivalent overloads for push-back behavior.
     *
     * @tparam T The element type the container holds.
     */
    template <typename Derived, typename T>
    class DirectlyPushable :
        public Pushable<Derived, T>
    {
    public:
        /**
         * @brief Append a value to the container by forwarding to EmplaceBack.
         *
         * This method provides a default PushBack implementation that forwards
         * to the Derived::EmplaceBack(...) method. The default template parameter
         * U = T allows usage with lvalues and rvalues.
         *
         * @tparam U Type of the supplied value (deduced). Defaults to T.
         * @param value The value to append (forwarded).
         */
        template <
            class U = T,
            typename = Trait::EnsureSame<T, U>
        >
        void PushBack(U &&value)
        {
            static_cast<Derived*>(this)->EmplaceBack(std::forward<U>(value));
        }
    };
}
