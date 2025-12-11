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

#include <celery/array/vector.h>

namespace Celery::Array
{
    namespace Pmr
    {
        /*
         * @brief A Stream class extending Vector to provide sequential access.
         *
         * This class allows for streaming access to elements in a Vector,
         * maintaining a read index to traverse the elements sequentially.
         *
         * @tparam T Element type stored in the Stream.
         * @tparam GrowthFactor Multiplicative growth factor used when expanding capacity.
         * @tparam InitialCapacity Initial number of elements to allocate.
         * @tparam Allocator Allocator type providing Allocate/Deallocate for T.
         *
         * @note Inherits from Celery::Array::Vector with the same template parameters.
         */
        template <
            typename T,
            Trait::Decimal GrowthFactor = Trait::GrowthFactor,
            Trait::Uint InitialCapacity = Trait::InitialCapacity,
            typename Allocator = Celery::Pmr::ArrayAllocator<T>
        >
        class Stream
            : public Vector<T, GrowthFactor, InitialCapacity, Allocator>
        {
        protected:
            // Current read index for streaming
            Trait::VeryLarge read_index = 0;

        public:
            // Inherit constructors from Vector base class
            using Vector<T, GrowthFactor, InitialCapacity, Allocator>::Vector;

            /*
             * @brief Get the next element in the stream and advance the read index.
             *
             * @return Reference to the next element.
             * @throws Except::OutOfRange when the read index exceeds the vector length.
             */
            T &Next()
            {
                if (read_index >= this->len)
                {
                    throw Except::OutOfRange();
                }

                return this->data[read_index++];
            }

            /*
             * @brief Get the previous element in the stream and move back the read index.
             *
             * @return Reference to the previous element.
             * @throws Except::OutOfRange when the read index is at the beginning.
             */
            T &Prev()
            {
                if (read_index == 0)
                {
                    throw Except::OutOfRange();
                }

                return this->data[--read_index];
            }

            /*
             * @brief Get the next element in the stream without changing the read index.
             *
             * @return Reference to the next element.
             * @throws Except::OutOfRange when the read index is out of bounds.
             */
            T &Peek()
            {
                if (read_index >= this->len)
                {
                    throw Except::OutOfRange();
                }

                return this->data[read_index];
            }

            /*
             * @brief Get the current element in the stream without changing the read index.
             *
             * @return Reference to the current element.
             * @throws Except::OutOfRange when the read index is out of bounds.
             */
            T &Curr()
            {
                if (read_index == 0 || read_index > this->len)
                {
                    throw Except::OutOfRange();
                }

                return this->data[read_index - 1];
            }

            /*
             * @brief Jump to the specified index in the stream.
             *
             * @param n The index to jump to.
             * @return Reference to the element at index n.
             * @throws Except::OutOfRange when n is out of bounds.
             */
            T &Nth(Trait::VeryLarge n)
            {
                if (n >= this->len)
                {
                    throw Except::OutOfRange();
                }

                read_index = n + 1;
                return this->data[n];
            }

            /*
             * @brief Check if there is a next element in the stream.
             *
             * @return true if there is a next element, false otherwise.
             */
            [[nodiscard]] bool HasNext() const noexcept
            {
                return read_index < this->len;
            }

            /*
             * @brief Check if there is a previous element in the stream.
             *
             * @return true if there is a previous element, false otherwise.
             */
            [[nodiscard]] bool HasPrev() const noexcept
            {
                return read_index > 0;
            }

            /*
             * @brief Set the current read position in the stream.
             *
             * @param pos The position to set the read index to.
             */
            void SetPos(size_t pos) noexcept
            {
                read_index = pos;
            }

            /*
             * @brief Get the current read position in the stream.
             *
             * @return The current read index.
             */
			size_t Pos() const noexcept
            {
                return read_index;
            }
        };
    }

    template <typename T>
    using Stream = Pmr::Stream<T>;
}