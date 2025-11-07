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
#include "celery/base/bufferable.h"
#include "celery/base/dereferenceable.h"
#include "celery/memory/monotonic.h"

namespace Celery::Ptr
{
    namespace Pmr
    {
        /**
         * @brief A unique ownership smart pointer with PMR-style allocator support.
         *
         * This class implements a simple unique pointer that manages the lifetime
         * of a dynamically allocated object using a specified PMR-style allocator.
         * It ensures that the object is properly deallocated when the Unique pointer
         * goes out of scope.
         *
         * @tparam T The type of the managed object.
         * @tparam Allocator Allocator type used to allocate/deallocate the managed object.
         *
         * @note SFINAE constraints enforce that \p Allocator derives from
         *       Celery::Pmr::Allocator<T>.
         */
        template<
            typename T,
            typename Allocator = Celery::Pmr::MonotonicAllocator<T>,
            // SFINAE to ensure Allocator is valid
            typename = Trait::EnsureAllocator<Allocator>
        >
        class Unique :
            public Base::Bufferable<T>,
            public Base::Dereferenceable<Unique<T, Allocator>, T>
        {
            // Make Dereferenceable a friend to access protected members
            friend class Base::Dereferenceable<Unique<T, Allocator>, T>;

        public:
            /**
             * @brief Construct a Unique pointer that takes ownership of the provided raw pointer.
             *
             * @param ptr Raw pointer to the dynamically allocated object.
             *
             * @note The Unique pointer assumes ownership of the provided pointer
             *       and will deallocate it using the specified Allocator when destroyed.
             */
            Unique(T *ptr)
            {
                if constexpr (std::is_array_v<T>)
                {
                    static_assert(
                        false,
                        "It is strongly discouraged to use Unique<T[]>; "
                        "please use Unique<Array::Vector<T>> or another container instead."
                        "Or, if you must, create a wrapper class around the array type."
                    );
                }

                // Allocate the object
                this->data = ptr;
            }

            /**
             * @brief Copy constructor and copy assignment are deleted to enforce unique ownership.
             */
            Unique(const Unique &) = delete;
            Unique &operator=(const Unique &) = delete;

            /**
             * @brief Move constructor transfers ownership from another Unique pointer.
             *
             * After the move, the source Unique pointer no longer owns the object.
             *
             * @param other The Unique pointer to move from.
             */
            Unique(Unique &&other)
            noexcept {
                // Move the pointer
                this->data = other.data;
                other.data = nullptr;
            }

            /**
             * @brief Destructor deallocates the managed object.
             */
            ~Unique()
            {
                if (this->data != nullptr)
                {
                    Allocator::Deallocate(this->data);
                    this->data = nullptr;
                }
            }
        };
    }

    template<typename T>
    using Unique = Pmr::Unique<T>;

    /**
     * @brief Create a Unique pointer with default PMR allocator.
     *
     * Allocates and constructs an object of type T using the specified
     * Allocator, then wraps it in a Unique pointer.
     *
     * @tparam T Type of the object to create.
     * @tparam Allocator Allocator type used to allocate the object.
     * @return Unique<T> A Unique pointer managing the newly created object.
     */
    template<
        typename T,
        typename Allocator = Celery::Pmr::MonotonicAllocator<T>
    >
    Pmr::Unique<T, Allocator> MakeUnique()
    {
        T *obj = Allocator::Allocate();
        // SFINAE checks are done by Unique, so we can safely ignore them here
        return { obj };
    }
}