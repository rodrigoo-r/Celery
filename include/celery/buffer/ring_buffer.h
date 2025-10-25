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
#include "celery/base/sizeable.h"
#include "celery/except/out_of_range.h"
#include "celery/memory/system.h"

namespace Celery::Buffer
{
    namespace Pmr
    {
        template <
        typename T,
        unsigned int Capacity = 50,
        bool UseHeap = Capacity >= 256,
        typename Allocator = Pmr::SystemArrayAllocator<T>,
        // SFINAE to ensure Capacity > 0
        typename = std::enable_if_t<(Capacity > 0)>,
        // SFINAE to ensure Allocator is a valid
        typename = std::enable_if_t<
            std::is_base_of_v<
                Pmr::ArrayAllocator<T>,
                Allocator
            >
        >
    >
    class RingBuffer : Base::Sizeable
        {
            // Data storage type
            using DataType = std::conditional_t<
                UseHeap,
                T*,
                T[Capacity]
            >;

            // Buffer storage
            DataType buffer;

        public:
            RingBuffer() : Sizeable()
            {
                // Allocate heap storage if needed
                if constexpr (UseHeap)
                {
                    buffer = Allocator::Allocate(Capacity);
                }
            }

            [[nodiscard]] size_t Head()
            const noexcept {
                return len;
            }

            template <typename ...Args>
            void EmplaceBack(Args &&...args)
            {
                if (len >= Capacity)
                {
                    // Wrap around
                    len = 0;
                }

                // Write in the next position
                if constexpr (std::is_trivially_copyable_v<T>)
                {
                    // Construct the object directly for trivially copyable types
                    buffer[len++] = T(std::forward<Args>(args)...);
                }
                else
                {
                    // Use placement new for non-trivially copyable types
                    new (&buffer[len++]) T(std::forward<Args>(args)...);
                }
            }

            template <class U>
            void Write(U &&val)
            {
                // Forward to emplace_back
                emplace_back(val);
            }

            T &operator[](const size_t index)
            {
                if (index >= len)
                {
                    throw Except::OutOfRangeException();
                }

                return buffer[index];
            }

            [[nodiscard]] bool Full() const
            {
                return len == Capacity;
            }

            void Clear()
            {
                // Reset length to zero
                len = 0;
            }

            ~RingBuffer()
            {
                // Deallocate heap storage if used
                if constexpr (UseHeap)
                {
                    Allocator::Deallocate(buffer);
                }
            }
        };
    }

    // Safe alias for RingBuffer with default parameters
    template <typename T, unsigned int Capacity = 50>
    using RingBuffer = Pmr::RingBuffer<T, Capacity>;
}