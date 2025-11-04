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
#include "iterator.h"

namespace Celery::Base
{
    /**
     * @brief CRTP mixin that provides begin()/end() iterators for buffered containers.
     *
     * This class assumes the derived class has `data` and `len` members
     * representing a contiguous buffer of elements of type `T`.
     *
     * @tparam Derived The final type that inherits from BufferedIterable\<Derived, T\>.
     *         Must have:
     *         - T* data: pointer to the start of the element buffer.
     *         - Trait::VeryLarge len: number of valid elements in the buffer.
     *
     * @tparam T The element type stored in the buffer.
     */
    template <typename Derived, typename T>
    class BufferedIterable
    {
        /**
         * @brief Return iterator to beginning of stored elements.
         *
         * @return Base::Iterable<T> Iterator pointing to start of data.
         */
        BufferedIterator<T> begin() const
        {
            return Base::BufferedIterator<T>(
                static_cast<const Derived*>(this)->data
            );
        }

        /**
         * @brief Return iterator to one-past-last stored element.
         *
         * @return Base::Iterable<T> Iterator pointing to end of stored data.
         */
        BufferedIterator<T> end() const
        {
            return Base::BufferedIterator<T>(
                static_cast<const Derived*>(this)->data +
                    static_cast<const Derived*>(this)->len
            );
        }
    };
}