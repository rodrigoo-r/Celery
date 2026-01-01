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
#include <atomic>


#include "Celery/Base/Bufferable.h"
#include "Celery/Base/Dereferenceable.h"
#include "Celery/Memory/Monotonic.h"

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
            typename = Trait::EnsureAllocator<RefCountAllocator>,
            typename = Trait::EnsureAllocator<Allocator>
        >
        class Shared :
            public Base::Bufferable<T>,
            public Base::Dereferenceable<
                Shared<
                    T,
                    ThreadSafe,
                    Allocator,
                    RefCountAllocator
                >,
                T
            >
        {
            // Make Dereferenceable a friend to access protected members
            friend class Base::Dereferenceable<
                Shared<
                    T,
                    ThreadSafe,
                    Allocator,
                    RefCountAllocator
                >,
                T
            >;

        protected:
            std::conditional_t<
                ThreadSafe,
                std::atomic<Trait::VeryLarge>,
                Trait::VeryLarge
            > *ref_count; // Pointer to reference count

        public:
            /**
             * @brief Constructs a Shared object from a raw pointer.
             *
             * This constructor takes ownership of a raw pointer and sets up
             * reference counting for shared ownership semantics. It assumes
             * that the pointer was allocated through the appropriate Allocator.
             *
             * @tparam T Type of the managed object.
             *
             * @param ptr Pointer to the object to manage. Must be allocated via Allocator.
             *
             * @note If T is an array type, compilation will fail with a static assertion,
             *       as using Shared<T[]> is discouraged. Use Shared<Array::Vector<T>>
             *       or another suitable container instead.
             *
             * @warning The constructor initializes the reference count to 1. Make sure
             *          not to pass an already managed pointer to avoid double-free errors.
             */
            Shared(T *ptr)
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

				// Only store if ptr is not null
				if (ptr == nullptr)
                {
                    // Null pointer case
                    this->data = nullptr;
                    ref_count = nullptr;
                }
				else
                {
					// Here, we assume ptr is a valid pointer allocated via Allocator
                	// Allocate the object
                	this->data = ptr;

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
            }

            /**
             * @brief Copy constructor increments the reference count.
             *
             * @param other The Shared pointer to copy from.
             */
            Shared(const Shared &other) :
                ref_count(other.ref_count)
            {
                // Copy the managed pointer
                this->data = other.data;

                // Increment reference count
                if constexpr (ThreadSafe)
                {
                    if (ref_count)
                        ref_count->fetch_add(1);
                } else
                {
                    if (ref_count)
                        (*ref_count)++;
                }
            }

            /**
             * @brief Move constructor transfers ownership without incrementing the count.
             *
             * @param other The Shared pointer to move from.
             */
            Shared(Shared &&other) :
                ref_count(other.ref_count)
            {
                this->data = other.data;
                other.data = nullptr;
                other.ref_count = nullptr;
            }

            /**
             * @brief Copy assignment operator.
             *
             * Decrements the current reference count and increments the
             * reference count of the assigned object.
             *
             * @param other The Shared pointer to assign from.
             * @return Reference to this Shared pointer.
             */
            Shared &operator=(Shared &&other)
            noexcept {
                if (this != &other)
                {
                    // Decrement current reference count
                    if (ref_count)
                    {
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
                            Allocator::Deallocate(this->data);
                            RefCountAllocator::Deallocate(ref_count);
                        }
                    }

                    // Move from other
                    this->data = other.data;
                    ref_count = other.ref_count;

                    other.data = nullptr;
                    other.ref_count = nullptr;
                }
                return *this;
            }

            /**
             * @brief Copy assignment operator.
             *
             * Decrements the current reference count and increments the
             * reference count of the assigned object.
             *
             * @param other The Shared pointer to assign from.
             * @return Reference to this Shared pointer.
             */
            Shared &operator=(const Shared &other)
            {
                if (this != &other)
                {
                    // Decrement current reference count
                    if (ref_count)
                    {
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
                            Allocator::Deallocate(this->data);
                            RefCountAllocator::Deallocate(ref_count);
                        }
                    }

                    // Copy from other
                    this->data = other.data;
                    ref_count = other.ref_count;

                    // Increment new reference count
                    if constexpr (ThreadSafe)
                    {
                        ref_count->fetch_add(1);
                    } else
                    {
                        ++(*ref_count);
                    }
                }
                return *this;
            }

            /**
             * @brief Equality operator to compare two Shared pointers.
             *
             * @param other The Shared pointer to compare with.
             * @return true if both point to the same object, false otherwise.
             */
            bool operator==(const Shared &other) const
            {
                return this->data == other.data;
            }

            /**
             * @brief Equality operator to compare with a raw pointer.
             *
             * @param ptr The raw pointer to compare with.
             * @return true if pointing to the same object, false otherwise.
             */
            bool operator==(T *ptr) const
            {
                return this->data == ptr;
            }

            /**
             * @brief Dereference operator to access the managed object.
             *
             * @return Pointer to the managed object.
             */
            T *operator->() const
            {
                return this->data;
            }

            /**
             * @brief Dereference operator to access the managed object.
             *
             * @return Reference to the managed object.
             */
            T &operator*() const
            {
                return *(this->data);
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
                        Allocator::Deallocate(this->data);
                        RefCountAllocator::Deallocate(ref_count);
                        this->data = nullptr;
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

    /**
     * @brief Create a non-thread-safe Shared pointer with default PMR allocators.
     *
     * Allocates and constructs an object of type T using the specified
     * Allocator, then wraps it in a Shared pointer with non-thread-safe
     * reference counting.
     *
     * @tparam T Type of the object to create.
     * @tparam Allocator Allocator type used to allocate the object.
     * @tparam RefCountAllocator Allocator type used to allocate the reference count.
     * @return Shared<T> A Shared pointer managing the newly created object.
     */
    template <
        typename T,
        typename Allocator = Celery::Pmr::MonotonicAllocator<T>,
        typename RefCountAllocator = Celery::Pmr::MonotonicAllocator<Trait::VeryLarge>,
        typename ...Args
    >
    Pmr::Shared<T, false, Allocator, RefCountAllocator> MakeShared(Args&&... args)
    {
        T *obj = Allocator::Allocate(std::forward<Args>(args)...);
        // SFINAE checks are done by Shared, so we can safely ignore them here
        return { obj };
    }

    /**
     * @brief Create a thread-safe Concurrent Shared pointer with default PMR allocators.
     *
     * Allocates and constructs an object of type T using the specified
     * Allocator, then wraps it in a Shared pointer with thread-safe
     * reference counting.
     *
     * @tparam T Type of the object to create.
     * @tparam Allocator Allocator type used to allocate the object.
     * @tparam RefCountAllocator Allocator type used to allocate the reference count.
     * @return Concurrent<T> A Concurrent Shared pointer managing the newly created object.
     */
    template <
        typename T,
        typename Allocator = Celery::Pmr::MonotonicAllocator<T>,
        typename RefCountAllocator = Celery::Pmr::MonotonicAllocator<std::atomic<Trait::VeryLarge>>,
        typename ...Args
    >
    Pmr::Shared<T, true, Allocator, RefCountAllocator> MakeConcurrent(Args&&... args)
    {
        T *obj = Allocator::Allocate(std::forward<Args>(args)...);
        // SFINAE checks are done by Shared, so we can safely ignore them here
        return { obj };
    }
}