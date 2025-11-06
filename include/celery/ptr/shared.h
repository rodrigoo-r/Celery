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
#include "celery/memory/monotonic.h"

namespace Celery::Ptr
{
    namespace Pmr
    {
        /**
         * @brief A reference-counted shared pointer with PMR-style allocators.
         *
         * This class implements a simple shared pointer that uses
         * reference counting to manage the lifetime of the pointed-to object.
         * It supports both thread-safe and non-thread-safe reference counting
         * based on the \p ThreadSafe template parameter.
         *
         * @tparam T The type of the managed object.
         * @tparam ThreadSafe If true, uses atomic reference counting for thread safety.
         * @tparam Allocator Allocator type used to allocate the managed object.
         * @tparam RefCountAllocator Allocator type used to allocate the reference count.
         *
         * @note SFINAE constraints enforce that \p Allocator derives from
         *       Celery::Pmr::MonotonicAllocator<T>.
         */
        template<
            typename T,
            bool ThreadSafe = false,
            typename Allocator = Celery::Pmr::MonotonicAllocator<T>,
            typename RefCountAllocator = Celery::Pmr::MonotonicAllocator<
                std::conditional_t<
                    ThreadSafe,
                    std::atomic<Trait::VeryLarge>,
                    Trait::VeryLarge
                >
            >,
            // SFINAE to ensure Allocator is valid
            typename = std::enable_if_t<
                std::is_base_of_v<
                    Celery::Pmr::MonotonicAllocator<T>,
                    Allocator
                >
            >
        >
        class Shared
        {
        protected:
            T *ptr; // Pointer to the managed object

            std::conditional_t<
                ThreadSafe,
                std::atomic<Trait::VeryLarge>,
                Trait::VeryLarge
            > *ref_count; // Pointer to reference count

        public:
            /**
             * @brief Constructs a new Shared pointer managing a new T instance.
             *
             * Forwards constructor arguments to T's constructor.
             *
             * @tparam Args Variadic template parameter pack for T's constructor arguments.
             * @param args Constructor arguments forwarded to T's constructor.
             */
            template<typename... Args>
            Shared(Args &&...args)
            {
                if constexpr (std::is_array_v<T>)
                {
                    static_assert(
                        false,
                        "It is strongly discouraged to use Shared<T[]>; "
                        "please use Shared<Array::Vector<T>> or another container instead."
                        "Or, if you must, create a wrapper class around the array type."
                    );
                }

                // Allocate the object
                ptr = Allocator::Allocate(std::forward<Args>(args)...);

                // Allocate and initialize the reference count
                ref_count = RefCountAllocator::Allocate();

                if constexpr (ThreadSafe)
                {
                    // Initialize atomic reference count to 1
                    ref_count->store(1);
                } else
                {
                    // Initialize non-atomic reference count to 1
                    *ref_count = 1;
                }
            }

            /**
             * @brief Copy constructor increments the reference count.
             *
             * @param other The Shared pointer to copy from.
             */
            Shared(const Shared &other) : ptr(other.ptr), ref_count(other.ref_count)
            {
                // Increment reference count
                if constexpr (ThreadSafe)
                {
                    ref_count->fetch_add(1);
                } else
                {
                    (*ref_count)++;
                }
            }

            /**
             * @brief Move constructor transfers ownership without incrementing the count.
             *
             * @param other The Shared pointer to move from.
             */
            Shared(Shared &&other) : ptr(other.ptr), ref_count(other.ref_count)
            {
                other.ptr = nullptr;
                other.ref_count = nullptr;
            }

            /**
             * @brief Dereference operator to access the managed object.
             *
             * @returns Reference to the managed object.
             */
            T &operator*() const
            {
                return *ptr;
            }

            /**
             * @brief Arrow operator to access members of the managed object.
             *
             * @returns Pointer to the managed object.
             */
            T *operator->() const
            {
                return ptr;
            }

            /**
             * @brief Destructor decrements the reference count and deallocates if zero.
             */
            ~Shared()
            {
                if (ref_count)
                {
                    // Decrement reference count
                    Trait::VeryLarge count;
                    if constexpr (ThreadSafe)
                    {
                        count = ref_count->fetch_sub(1) - 1;
                    } else
                    {
                        count = --(*ref_count);
                    }

                    // If count reaches zero, deallocate
                    if (count == 0)
                    {
                        Allocator::Deallocate(ptr);
                        RefCountAllocator::Deallocate(ref_count);
                        ptr = nullptr;
                        ref_count = nullptr;
                    }
                }
            }
        };
    }

    template<typename T, bool ThreadSafe = false>
    using Shared = Pmr::Shared<T, ThreadSafe>;

    template<typename T>
    using Concurrent = Pmr::Shared<T, true>;
}