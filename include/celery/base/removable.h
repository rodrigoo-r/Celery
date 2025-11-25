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

#include "celery/trait/default.h"
#include "celery/util/reindex.h"

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
            typename = Trait::EnsureSame<T, U>
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
            typename = Trait::EnsureSame<T, U>
        >
        void Erase(U &&value)
        {
            static_cast<Derived*>(this)->Remove(std::forward<U>(value));
        }
    };

    /**
     * @brief CRTP mixin that provides buffered removal implementations.
     *
     * This class extends Removable and provides default implementations
     * for removing elements by index, by value, or by pointer. It assumes
     * that the derived class has `data` and `len` members representing
     * the internal storage and current length.
     *
     * @tparam Derived The final type that inherits from BufferedRemovable\<Derived, T\>.
     *         Must have `data` and `len` members.
     *
     * @tparam T The element type the container holds.
     */
    template <class Derived, typename T>
    class BufferedRemovable :
        public Removable<Derived, T>
    {
    public:
        /**
         * @brief Remove an element at the specified index.
         *
         * Reindexes the internal data to remove the element at `idx`.
         * Throws `Except::OutOfRange` if `idx` is out of bounds.
         *
         * @param idx Index of the element to remove.
         * @throws Except::OutOfRange When `idx >= len`.
         */
        template <class U = Trait::VeryLarge>
        void RemoveAt(U &&idx)
        {
            auto &data = static_cast<Derived*>(this)->data;
            auto &len = static_cast<Derived*>(this)->len;
            Utility::Reindex(data, len, idx);
            --this->len; // Decrease length after removal
        }

        /**
         * @brief Remove the first occurrence of a value.
         *
         * Searches for the first occurrence of `value` and removes it
         * by reindexing the internal data. If the value is not found,
         * no action is taken.
         *
         * @param value The value to remove.
         */
        template <
            class U = T,
            typename = Trait::EnsureSame<T, U>
        >
        void Remove(const U &value)
        {
            auto &data = static_cast<Derived*>(this)->data;
            auto &len = static_cast<Derived*>(this)->len;

            for (Trait::VeryLarge i = 0; i < len; ++i)
            {
                if (data[i] == value)
                {
                    Utility::Reindex(data, len, i);
                    --static_cast<Derived*>(this)->len; // Decrease length after removal
                    return; // Remove only the first occurrence
                }
            }
        }

        /**
         * @brief Remove an element by pointer.
         *
         * Calculates the index of the element pointed to by `ptr`
         * and removes it by reindexing. Throws `Except::OutOfRange`
         * if the pointer is out of bounds.
         *
         * @param ptr Pointer to the element to remove.
         * @throws Except::OutOfRange When `ptr` is out of bounds.
         */
        template <
            class U = T,
            typename = Trait::EnsureSame<T, U>
        >
        void Erase(U *&&ptr)
        {
            auto &data = static_cast<Derived*>(this)->data;

            // Calculate index from pointer
            const auto index = static_cast<Trait::VeryLarge>(ptr - data);
            RemoveAt(index);
        }
    };
}