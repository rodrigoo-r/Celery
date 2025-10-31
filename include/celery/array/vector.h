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
#include "celery/base/resizable.h"
#include "celery/memory/base.h"
#include "celery/memory/system.h"
#include "celery/trait/default.h"
#include "celery/util/copy.h"

namespace Celery::Array
{
    namespace Pmr
    {
        template <
            typename T,
            Trait::Decimal GrowthFactor = Trait::GrowthFactor,
            Trait::Uint InitialCapacity = Trait::InitialCapacity,
            typename Allocator = Celery::Pmr::SystemArrayAllocator<T>,
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
            typename = std::enable_if_t<
                std::is_base_of_v<
                    Celery::Pmr::ArrayAllocator<T>,
                    Allocator
                >
            >
        >
        class Vector : public Base::Indexable<T>, public Base::Resizable
        {
            void Init()
            {
                // Allocate initial capacity
                capacity = static_cast<Trait::VeryLarge>(InitialCapacity);
                this->data = Allocator::Allocate(capacity);
            }

        public:
            Vector() : Base::Indexable<T>(), Resizable()
            {
                this->Init();
            }

            Vector(std::initializer_list<T> init_list) : Base::Indexable<T>(), Resizable()
            {
                this->Init();

                // Insert each element from the initializer list
                for (const auto &elem : init_list)
                {
                    EmplaceBack(elem);
                }
            }

            Vector &operator=(const Vector &other)
            {
                if (this != &other)
                {
                    // Clear current contents
                    Clear();

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

            void Clear()
            {
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

            void Reset()
            {
                // Clear the vector
                Clear();

                // Deallocate current data
                Allocator::Deallocate(this->data);

                // Re-initialize
                this->Init();
            }

            void Resize(Trait::VeryLarge new_capacity)
            override {
                if (new_capacity <= capacity) return;

                // Allocate new data
                T *new_data = Allocator::Allocate(new_capacity);

                // Copy existing elements to new data
                Utility::Copy(
                    this->data,
                    new_data,
                    this->len
                );

                // Deallocate old data
                Allocator::Deallocate(this->data);

                // Update data pointer and capacity
                this->data = new_data;
                capacity = new_capacity;
            }

            void EnsureGrowth(const Trait::VeryLarge cap)
            {
                if (cap <= capacity) return; // Already enough capacity

                auto new_cap = capacity;

                // Grow until we reach the required capacity
                while (new_cap < cap)
                {
                    new_cap = static_cast<Trait::VeryLarge>(new_cap * GrowthFactor);
                }

                // Resize to the new capacity
                Resize(new_cap);
            }

            void EmplaceBack(const T &value)
            {
                // Resize if necessary
                if (this->len >= capacity)
                {
                    Resize(static_cast<Trait::VeryLarge>(capacity * GrowthFactor));
                }

                // Construct the new element in place
                new (this->data + this->len) T(value);
                ++this->len;
            }

            void PushBack(T &&value)
            {
                // Move the value into place
                EmplaceBack(std::move(value));
            }

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

            ~Vector() override
            {
                // Call the destructor for each element
                Clear();

                // Deallocate the data
                Allocator::Deallocate(this->data);
            }
        };
    }

    template <typename T>
    using Vector = Pmr::Vector<T>;
}