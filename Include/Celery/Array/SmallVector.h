/*
        ==== The Zelix Programming Language ====
---------------------------------------------------------
  - This file is part of the Zelix Programming Language
    codebase. Zelix is a fast, statically-typed and
    memory-safe programming language that aims to
    match native speeds while staying highly ergonomic.
---------------------------------------------------------
  - Zelix is categorized as free software; you can
    redistribute it and/or modify it under the terms of
    the Apache License, Version 2.0.
---------------------------------------------------------
  - You should have received a copy of the Apache
    License v2.0 along with Zelix. If not, see
    <http://www.apache.org/licenses/LICENSE-2.0>.
*/

//
// Created by Rodrigo on 10/02/26.
//

#pragma once

#include <initializer_list>


#include "Celery/Base/Indexable.h"
#include "Celery/Base/Iterable.h"
#include "Celery/Base/Pushable.h"
#include "Celery/Base/Removable.h"
#include "Celery/Base/Resizable.h"
#include "Celery/Memory/System.h"
#include "Celery/Trait/Default.h"
#include "Celery/Trait/Type.h"
#include "Celery/Util/Copy.h"
#include "Celery/Array/Vector.h"

namespace Celery::Array
{
    namespace Pmr
    {
        /**
         * @brief A small vector implementation that optimizes for small sizes by using stack allocation.
         *
         * The SmallVector class is a dynamic array that uses stack allocation for small sizes (up to HeapThreshold)
         * and switches to heap allocation when the size exceeds the threshold. This design minimizes heap allocations
         * for small vectors, improving performance in common cases while still supporting larger sizes when needed.
         *
         * @tparam T The type of elements stored in the vector.
         * @tparam HeapThreshold The maximum number of elements that can be stored on the stack before switching to heap allocation.
         * @tparam GrowthFactor The factor by which the capacity grows when resizing is needed.
         * @tparam InitialCapacity The initial capacity of the vector when heap allocation is used.
         * @tparam Allocator The allocator type used for heap allocations.
         */
        template <
            typename T,
            Trait::Uint HeapThreshold = 8,
            Trait::Decimal GrowthFactor = Trait::GrowthFactor,
            Trait::Uint InitialCapacity = Trait::InitialCapacity,
            typename Allocator = Celery::Pmr::ArrayAllocator<T>,
            // --- SFINAE Checks --- //
            typename = Trait::EnsureArrayAllocator<Allocator>
        >
        class SmallVector :
            public Base::Indexable<T>,
            public Base::Resizable,
            public Base::Pushable<Vector<T, GrowthFactor, InitialCapacity, Allocator>, T>,
            public Base::BufferedRemovable<Vector<T, GrowthFactor, InitialCapacity, Allocator>, T>,
            public Base::BufferedIterable<Vector<T, GrowthFactor, InitialCapacity, Allocator>, T>,
            public BufferedBase<Vector<T, GrowthFactor, InitialCapacity, Allocator>, T, GrowthFactor>
        {
            // Make Base::BufferedRemovable and Base::BufferedIterable friends to access protected members
            friend class Base::BufferedRemovable<Vector<T, GrowthFactor, InitialCapacity, Allocator>, T>;
            friend class Base::BufferedIterable<Vector<T, GrowthFactor, InitialCapacity, Allocator>, T>;

        protected:
            alignas(T) T stack[HeapThreshold];
            T *data = stack;
            bool on_heap = false;

            void Init()
            {
                // Allocate initial capacity
                capacity = static_cast<Trait::VeryLarge>(InitialCapacity);
                this->data = Allocator::Allocate(capacity);
                on_heap = true;

                if (this->len > 0)
                {
                    Utility::Copy(stack, this->data, this->len);
                }
            }

            template<bool IsConstruct = false>
            void ConstructFrom(const SmallVector &other)
            {
                // Only check self-assignment for assignment operations
                if constexpr (!IsConstruct)
                {
                    if (this == &other) return; // Self-assignment check
                    this->Clear(); // Clear current contents
                }

                this->len = other.len;
                this->capacity = other.capacity;

                if (this->len <= HeapThreshold)
                {
                    this->data = stack;
                    Utility::Copy(other.data, this->data, this->len);
                    this->on_heap = false;
                }
                else
                {
                    this->data = Allocator::Allocate(capacity);
                    this->on_heap = true;
                }

                Utility::Copy(other.data, this->data, this->len);
            }

            template<bool IsConstruct = false>
            void ConstructFrom(SmallVector &&other)
            {
                // Only check self-assignment for assignment operations
                if constexpr (!IsConstruct)
                {
                    if (this == &other) return; // Self-assignment check
                    this->Clear(); // Clear current contents
                }

                this->len = other.len;
                this->capacity = other.capacity;

                if (other->on_heap)
                {
                    this->data = other.data;
                    this->on_heap = true;
                }
                else
                {
                    this->data = stack;
                    Utility::Copy(other.data, this->data, this->len);
                    this->on_heap = false;
                }

                other.data = nullptr;
                other.len = 0;
                other.capacity = 0;
            }

            using BufferedBaseType =
                BufferedBase<
                    Vector<
                        T,
                        GrowthFactor,
                        InitialCapacity,
                        Allocator
                    >,
                    T,
                    GrowthFactor
                >;
        public:
            using BufferedBaseType::BufferedBaseType; // Inherit constructors from BufferedBase

        	SmallVector& operator=(const SmallVector &other) noexcept
        	{
        		if (this != &other)
        			ConstructFrom(other);
        		return *this;
        	}

        	SmallVector &operator=(SmallVector &&other) noexcept
        	{
        		if (this != &other)
        			ConstructFrom(std::move(other));
        		return *this;
        	}

            /**
             * @brief Default constructor.
             *
             * Initializes the internal storage and sets length to zero (via
             * the Base::Indexable and Base::Resizable base initializations).
             */
            SmallVector() :
        		Base::Indexable<T>(),
        		Resizable()
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
        	SmallVector(std::initializer_list<T> init_list) :
				Base::Indexable<T>(),
				Resizable()
            {
	            ConstructFrom(init_list);
            }

        	/**
			* @brief Copy constructor.
			*
			* Creates a new vector as a copy of \p other by allocating
			* sufficient storage and copying each element.
			*
			* @param other Source vector to copy from.
			*/
        	SmallVector(const SmallVector &other) :
				Base::Indexable<T>(),
				Resizable()
            {
            	ConstructFrom(other);
            }

        	/**
			* @brief Move constructor.
			*
			* Transfers ownership of resources from \p other to this vector,
			* leaving \p other in a valid but empty state.
			*
			* @param other Source vector to move from.
			*/
        	SmallVector(SmallVector &&other) :
				Base::Indexable<T>(),
				Resizable()
            {
            	ConstructFrom(std::move(other));
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
                this->Clear();

                // Deallocate current data
        	    if (this->on_heap)
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
                if (
                    (on_heap && new_capacity <= HeapThreshold) ||
                    (!on_heap && new_capacity <= capacity)
                ) return;

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

                    // Destroy old elements before freeing
                    if constexpr (!std::is_trivially_destructible_v<T>)
                        for (Trait::VeryLarge i = 0; i < this->len; ++i)
                            this->data[i].~T();
                }

                // Deallocate old data
        	    if (this->on_heap)
                    Allocator::Deallocate(this->data);

                // Update data pointer and capacity
                this->data = new_data;
                capacity = new_capacity;
            }

            /**
             * @brief Destructor.
             *
             * Clears elements (invoking destructors as needed) and deallocates
             * the allocated storage via the chosen Allocator.
             */
            ~SmallVector() override
            {
                if (!this->data) return; // Nothing to do
                // Call the destructor for each element
                this->Clear();

                // Deallocate the data
        	    if (this->on_heap)
                    Allocator::Deallocate(this->data);

                this->data = nullptr;
            }
        };
    }

    template <
        typename T,
        Trait::Uint HeapThreshold = 8
    >
    using SmallVector = Pmr::SmallVector<
        T,
        HeapThreshold
    >;
}