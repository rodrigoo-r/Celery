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
#include <initializer_list>


#include "celery/base/indexable.h"
#include "celery/base/iterable.h"
#include "celery/base/pushable.h"
#include "celery/base/removable.h"
#include "celery/base/resizable.h"
#include "celery/memory/system.h"
#include "celery/trait/default.h"
#include "celery/trait/type.h"
#include "celery/util/copy.h"

#pragma once

namespace Celery::Array
{
    namespace Pmr
    {
        /**
         * @brief A resizable dynamic array with PMR-style allocator support.
         *
         * This class models a high-performance vector container similar to
         * std::vector but tailored for Celery's allocator and trait system.
         *
         * @tparam T Element type stored in the Vector.
         * @tparam GrowthFactor Multiplicative growth factor used when expanding capacity.
         * @tparam InitialCapacity Initial number of elements to allocate.
         * @tparam Allocator Allocator type providing Allocate/Deallocate for T.
         *
         * @note Template SFINAE constraints:
         * - \c T must be copy-constructible, move-constructible, and destructible.
         * - \c Allocator must be derived from Celery::Pmr::ArrayAllocator<T>.
         */
        template <
            typename T,
            Trait::Decimal GrowthFactor = Trait::GrowthFactor,
            Trait::Uint InitialCapacity = Trait::InitialCapacity,
            typename Allocator = Celery::Pmr::ArrayAllocator<T>,
            // --- SFINAE Checks --- //
            typename = std::enable_if_t<
                std::is_copy_constructible_v<T>
            >,
            typename = std::enable_if_t<
                std::is_move_constructible_v<T>
            >,
            typename = std::enable_if_t<
                std::is_destructible_v<T>
            >,
            typename = Trait::EnsureArrayAllocator<Allocator>
        >
        class Vector :
            public Base::Indexable<T>,
            public Base::Resizable,
            public Base::Pushable<Vector<T, GrowthFactor, InitialCapacity, Allocator>, T>,
            public Base::BufferedRemovable<Vector<T, GrowthFactor, InitialCapacity, Allocator>, T>,
            public Base::BufferedIterable<Vector<T, GrowthFactor, InitialCapacity, Allocator>, T>
        {
            // Make Base::BufferedRemovable and Base::BufferedIterable friends to access protected members
            friend class Base::BufferedRemovable<Vector<T, GrowthFactor, InitialCapacity, Allocator>, T>;
            friend class Base::BufferedIterable<Vector<T, GrowthFactor, InitialCapacity, Allocator>, T>;

            /**
             * @brief Initialize internal storage with the configured initial capacity.
             *
             * Allocates memory for \c InitialCapacity elements using the provided
             * \c Allocator and sets the internal capacity value.
             */
            void Init()
            {
                // Allocate initial capacity
                capacity = static_cast<Trait::VeryLarge>(InitialCapacity);
                this->data = Allocator::Allocate(capacity);
            }

        public:
            /**
             * @brief Default constructor.
             *
             * Initializes the internal storage and sets length to zero (via
             * the Base::Indexable and Base::Resizable base initializations).
             */
            Vector() : Base::Indexable<T>(), Resizable()
            {
                // Vector is lazily initialized, thus
                // no allocation is done here.
                this->len = 0;
            }

            /**
             * @brief Construct from an initializer list.
             *
             * Each element in \p init_list is copied/emplaced into the vector.
             *
             * @param init_list Initial values to insert.
             */
            Vector(std::initializer_list<T> init_list) : Base::Indexable<T>(), Resizable()
            {
                this->Init();

                // Insert each element from the initializer list
                for (const auto &elem : init_list)
                {
                    EmplaceBack(elem);
                }
            }

            /**
             * @brief Copy assignment operator.
             *
             * Clears current contents, ensures sufficient capacity, and copies
             * elements from \p other to this vector.
             *
             * @param other Source vector to copy from.
             * @return Reference to this vector.
             */
            Vector &operator=(const Vector &other)
            {
                if (this != &other)
                {
                    // Clear current contents
                    Clear();
                    if (other.len == 0) return *this; // Nothing to copy

                    // Ensure capacity
                    EnsureGrowth(other.len);

                    // Copy elements from other
                    Utility::Copy(
                        other.data,
                        this->data,
                        other.len
                    );
                }
                return *this;
            }

            /**
             * @brief Clear all elements from the vector.
             *
             * Calls destructors for non-trivially-destructible elements and
             * resets the stored length to zero. Does not deallocate capacity.
             */
            void Clear()
            {
                if (this->len == 0) return; // Already empty

                // Call the destructor for each element if necessary
                if constexpr (!std::is_trivially_destructible_v<T>)
                {
                    for (Trait::VeryLarge i = 0; i < this->len; ++i)
                    {
                        this->data[i].~T();
                    }
                }

                // Reset length
                this->len = 0;
            }

            /**
             * @brief Reset the vector to initial state.
             *
             * Clears contents, deallocates the current storage, and
             * re-initializes allocation to the configured initial capacity.
             */
            void Reset()
            {
                // Clear the vector
                Clear();

                // Deallocate current data
                Allocator::Deallocate(this->data);

                // Re-initialize
                this->Init();
            }

            /**
             * @brief Resize internal storage to at least \p new_capacity.
             *
             * If \p new_capacity is less than or equal to current capacity, the
             * call is a no-op. Otherwise allocates new storage, copies existing
             * elements, deallocates old storage, and updates internal state.
             *
             * @param new_capacity Desired new capacity (number of element slots).
             */
            void Resize(Trait::VeryLarge new_capacity)
            override {
                if (!this->data) Init(); // Lazy initialization
                if (new_capacity <= capacity) return;

                // Allocate new data
                T *new_data = Allocator::Allocate(new_capacity);

                // Copy existing elements to new data
                if (this->len != 0)
                {
                    Utility::Copy(
                        this->data,
                        new_data,
                        this->len
                    );
                }

                // Deallocate old data
                Allocator::Deallocate(this->data);

                // Update data pointer and capacity
                this->data = new_data;
                capacity = new_capacity;
            }

            /**
             * @brief Ensure capacity is at least \p cap, growing by the configured factor.
             *
             * Repeatedly multiplies the current capacity by \c GrowthFactor until
             * it meets or exceeds \p cap, then resizes to that capacity.
             *
             * @param cap Minimum required capacity.
             */
            void EnsureGrowth(const Trait::VeryLarge cap)
            {
                if (!this->data) Init(); // Lazy initialization

                const auto required = static_cast<Trait::VeryLarge>(this->len + cap);
                if (required <= capacity) return; // Already enough capacity

                auto new_cap = capacity;

                // Grow until we reach the required capacity
                while (new_cap < required)
                {
                    new_cap = static_cast<Trait::VeryLarge>(new_cap * GrowthFactor);
                }

                // Resize to the new capacity
                Resize(new_cap);
            }

            /**
             * @brief Construct an element at the end of the vector by copying \p value.
             *
             * If necessary, the vector will grow using the configured growth factor.
             *
             * @param args Arguments forwarded to T's constructor.
             */
            template <typename ...Args>
            void EmplaceBack(Args&&... args)
            {
                // Resize if necessary
                EnsureGrowth(1);

                // Construct the new element in place
                new (&this->data[this->len]) T(std::forward<Args>(args)...);
                ++this->len;
            }

            /**
             * @brief Push an element to the back of the vector.
             *
             * This forwards to EmplaceBack using move semantics.
             *
             * @param value Rvalue reference to the element to push.
             */
            template <
                class U = T,
                typename = Trait::EnsureSame<T, U>
            >
            void PushBack(U &&value)
            {
                if constexpr (!std::is_trivially_constructible_v<T>)
                {
                    // Move the value into place
                    EmplaceBack(std::forward<U>(value));
                }
                else
                {
                    // For trivially constructible types, we can optimize
                    // by directly assigning the value after ensuring capacity.
                    EnsureGrowth(1);
                    this->data[this->len] = std::forward<U>(value);
                    ++this->len;
                }
            }

            /**
             * @brief Remove the last element from the vector.
             *
             * Throws Except::OutOfRange if the vector is empty. For non-trivial
             * destructible types, the destructor is invoked.
             *
             * @throws Except::OutOfRange when the vector has zero elements.
             */
            void PopBack()
            {
                if (this->len == 0)
                {
                    throw Except::OutOfRange();
                }

                // Decrease length
                --this->len;

                // Only call destructor if T is not trivially destructible
                if constexpr (!std::is_trivially_destructible_v<T>)
                {
                    this->data[this->len].~T();
                }
            }

            /**
             * @brief Remove the last element and return it by reference for move semantics.
             *
             * Throws Except::OutOfRange if the vector is empty. Does not call the
             * destructor, allowing the caller to move from the returned reference.
             *
             * @return Reference to the removed element for move semantics.
             * @throws Except::OutOfRange when the vector has zero elements.
             */
            T &PopBackMove()
            {
                if (this->len == 0)
                {
                    throw Except::OutOfRange();
                }

                // Decrease length
                --this->len;

                // Return the element by reference for move semantics
                return this->data[this->len];
            }

            /**
             * @brief Destructor.
             *
             * Clears elements (invoking destructors as needed) and deallocates
             * the allocated storage via the chosen Allocator.
             */
            ~Vector() override
            {
                if (!this->data) return; // Nothing to do
                // Call the destructor for each element
                Clear();

                // Deallocate the data
                Allocator::Deallocate(this->data);
            }
        };
    }

    /**
     * @brief Convenience alias for PMR Vector in the Celery::Array namespace.
     *
     * This alias exposes the PMR-based Vector with default template parameters.
     *
     * @tparam T Element type.
     */
    template <typename T>
    using Vector = Pmr::Vector<T>;
}
