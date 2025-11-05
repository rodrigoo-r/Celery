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
#include "celery/array/vector.h"
#include "celery/list/list.h"
#include "celery/memory/system.h"
#include "celery/trait/default.h"

namespace Celery::Pmr
{
    /**
     * @class BumpPage
     * @brief Represents a single page of bump-allocated memory.
     *
     * A `BumpPage` is a fixed-size block of memory that allocates objects sequentially
     * without individual frees until destruction. Ideal for fast, temporary allocations.
     *
     * @tparam T Type of elements stored.
     * @tparam PageSize Number of elements per page (default: 256).
     * @tparam BufferAllocator Allocator used for raw memory (default: ArrayAllocator<T>).
     */
    template <
        typename T,
        Trait::Uint PageSize = 256,
        typename BufferAllocator = ArrayAllocator<T>,
        typename = std::enable_if_t<
            std::is_base_of_v<ArrayAllocator<T>, BufferAllocator>
        >
    >
    class BumpPage
    {
    protected:
        Trait::VeryLarge size = sizeof(T) * PageSize; /**< Total size of the page in bytes. */
        Trait::VeryLarge used = 0;                    /**< Number of bytes currently used. */
        T *data;                                      /**< Pointer to page memory. */

    public:
        /**
         * @brief Constructs a new page and allocates memory using the buffer allocator.
         */
        BumpPage()
        {
            data = BufferAllocator::Allocate(PageSize);
        }

        /**
         * @brief Checks if the page is full.
         * @return True if no more space is available.
         */
        [[nodiscard]] bool Full() const
        {
            return used >= size;
        }

        /**
         * @brief Allocates and constructs a new object in-place on the page.
         *
         * @tparam Args Parameter pack for T's constructor.
         * @param args Arguments forwarded to T's constructor.
         * @return Pointer to the constructed object, or nullptr if the page is full.
         */
        template <typename ...Args>
        T *Bump(Args &&...args)
        {
            if (used + sizeof(T) > size)
            {
                return nullptr;
            }

            T *ptr = reinterpret_cast<T *>(reinterpret_cast<char *>(data) + used);
            new (ptr) T(std::forward<Args>(args)...);
            used += sizeof(T);
            return ptr;
        }

        /**
         * @brief Destroys all constructed objects (if needed) and releases memory.
         */
        ~BumpPage()
        {
            if constexpr (!std::is_trivially_destructible_v<T>)
            {
                const Trait::VeryLarge count = used / sizeof(T);
                for (Trait::VeryLarge i = 0; i < count; ++i)
                {
                    data[i].~T();
                }
            }

            BufferAllocator::Deallocate(data);
        }
    };

    /**
     * @class BumpAllocator
     * @brief Manages multiple `BumpPage` instances and recycles freed memory.
     *
     * Provides a layer above `BumpPage` to handle multiple pages and a free list
     * for reuse of previously deallocated memory.
     *
     * @tparam T Type of elements stored.
     * @tparam PageSize Number of elements per page.
     * @tparam FreeListAllocator Allocator for managing the free list.
     * @tparam BufferAllocator Allocator used for page buffers.
     * @tparam PageAllocator Allocator used for page list nodes.
     */
    template <
        typename T,
        Trait::Uint PageSize = 256,
        typename FreeListAllocator = Allocator<
            List::Internal::LinkedListNode<T *>
        >,
        typename BufferAllocator = ArrayAllocator<T>,
        typename PageAllocator = Allocator<
            List::Internal::LinkedListNode<
                BumpPage<T, PageSize, BufferAllocator>
            >
        >
    >
    class BumpAllocator
    {
    protected:
        List::Pmr::LinkedList<T *, FreeListAllocator> free_list; /**< List of reusable objects. */
        List::Pmr::LinkedList<BumpPage<T, PageSize, BufferAllocator>, PageAllocator> pages; /**< Managed memory pages. */

    public:
        /**
         * @brief Allocates and constructs an object of type T.
         *
         * Uses a free list first if available, otherwise allocates from the current page.
         * Creates new pages automatically when needed.
         *
         * @tparam Args Parameter pack for T's constructor.
         * @param args Arguments forwarded to T's constructor.
         * @return Pointer to the newly allocated object.
         * @throws Except::BadAlloc If no memory can be allocated.
         */
        template <typename ...Args>
        T *Allocate(Args &&...args)
        {
            if (free_list.Len() > 0)
            {
                auto node = free_list.PopBackMove();
                if constexpr (!std::is_trivially_destructible_v<T>)
                {
                    node->~T();
                }

                new (node) T(std::forward<Args>(args)...);
                return node;
            }

            if (pages.Empty() || pages.Back().Full())
            {
                pages.EmplaceBack();
            }

            if (T *ptr = pages.Back().Bump(std::forward<Args>(args)...))
            {
                return ptr;
            }

            throw Except::BadAlloc();
        }

        /**
         * @brief Marks a memory slot for reuse.
         *
         * Currently disabled; can be implemented to return memory to free_list.
         *
         * @param ptr Pointer to the object to deallocate.
         */
        void Deallocate(T *ptr)
        {
            //free_list.EmplaceBack(ptr);
        }

        /**
         * @brief Clears the free list on destruction.
         */
        ~BumpAllocator()
        {
            free_list.AggressiveClear();
        }
    };

    /**
     * @class MonotonicAllocator
     * @brief Thread-local allocator that only grows and never frees.
     *
     * A wrapper around `BumpAllocator` providing a static, thread-local allocator.
     * Perfect for monotonic allocation patterns like ECS systems or arenas.
     *
     * @tparam T Type of elements stored.
     * @tparam PageSize Number of elements per page.
     * @tparam FreeListAllocator Allocator for managing the free list.
     * @tparam BufferAllocator Allocator used for page buffers.
     * @tparam PageAllocator Allocator used for page list nodes.
     */
    template <
        typename T,
        Trait::Uint PageSize = 256,
        typename FreeListAllocator = Allocator<
            List::Internal::LinkedListNode<T *>
        >,
        typename BufferAllocator = ArrayAllocator<T>,
        typename PageAllocator = Allocator<
            List::Internal::LinkedListNode<
                BumpPage<T, PageSize, BufferAllocator>
            >
        >
    >
    class MonotonicAllocator : Allocator<T>
    {
    protected:
        static inline thread_local BumpAllocator<
            T, PageSize, FreeListAllocator, BufferAllocator, PageAllocator
        > bump_allocator; /**< Thread-local bump allocator instance. */

    public:
        /**
         * @brief Allocates and constructs an object using a monotonic strategy.
         * @tparam Args Parameter pack for T's constructor.
         * @param args Arguments forwarded to T's constructor.
         * @return Pointer to the newly constructed object.
         */
        template <typename ...Args>
        static inline T *Allocate(Args &&...args)
        {
            return bump_allocator.Allocate(std::forward<Args>(args)...);
        }

        /**
         * @brief Destroys and returns memory to the bump allocator.
         *
         * If T is not trivially destructible, its destructor is called before freeing.
         *
         * @param ptr Pointer to object to destroy and deallocate.
         */
        static inline void Deallocate(T *ptr)
        {
            bump_allocator.Deallocate(ptr);
        }
    };
}