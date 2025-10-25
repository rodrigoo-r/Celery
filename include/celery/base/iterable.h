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

namespace Celery::Base
{
    /**
     * @brief Lightweight iterable wrapper for raw buffers.
     *
     * Template parameter `It` represents the iterator or pointer type
     * used to traverse a contiguous buffer.
     *
     * @tparam It Iterator or pointer type for the underlying buffer.
     */
    template<typename It>
    class Iterable
    {
        /**
         * @brief Pointer to the current position within the buffer.
         *
         * This points to an element of the underlying contiguous storage
         * and is advanced by operator++.
         */
        It *buffer; // The buffer to iterate over

    public:
        /**
         * @brief Construct an Iterable starting at \p buf.
         *
         * @param buf Pointer to the initial element in the buffer.
         */
        Iterable(It *buf) : buffer(buf)
        {}

        /**
         * @brief Dereference the iterable.
         *
         * Returns the current iterator/value at the internal buffer pointer.
         * The exact semantics depend on the template parameter `It`.
         *
         * @return The current iterator or value.
         */
        It operator*()
        {
            return buffer;
        }

        /**
         * @brief Pre-increment the iterable to the next element.
         *
         * Advances the internal buffer pointer and returns a reference to
         * this iterable to allow chaining.
         *
         * @return Reference to this iterable after increment.
         */
        Iterable& operator++()
        {
            ++buffer;
            return *this;
        }

        /**
         * @brief Compare two iterables for inequality.
         *
         * Two iterables are considered different if their internal buffer
         * pointers do not point to the same position.
         *
         * @param other Another iterable to compare with.
         * @return true if the iterables are at different positions.
         */
        bool operator!=(const Iterable& other) const
        {
            return buffer != other.buffer;
        }
    };
}