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

#include "Celery/Base/Indexable.h"
#include "Celery/Base/Iterable.h"
#include "Celery/Base/Pushable.h"
#include "Celery/Base/Removable.h"
#include "Celery/Base/Resizable.h"
#include "Celery/Memory/System.h"
#include "Celery/Trait/Default.h"
#include "Celery/Trait/Type.h"
#include "Celery/Util/Copy.h"

#pragma once

namespace Celery::Array
{
	template <
		typename Derived,
        typename T,
        Trait::Decimal GrowthFactor
    >
	class BufferedBase
	{
	public:
		BufferedBase() = default;

        /**
        * @brief Assignment from an initializer list.
        *
        * Clears current contents and inserts each element from \p init_list.
        *
        * @param init_list Initial values to insert.
        * @return Reference to this vector.
        */
        Derived &operator=(std::initializer_list<T> init_list)
        {
            ConstructFrom(init_list);
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
        	if (static_cast<Derived *>(this)->len == 0)
            	return; // Already empty

	        // Call the destructor for each element if necessary
    	    if constexpr (!std::is_trivially_destructible_v<T>)
        	{
            	for (Trait::VeryLarge i = 0; i < static_cast<Derived *>(this)->len; ++i)
            	{
                	static_cast<Derived *>(this)->data[i].~T();
            	}
        	}

	        // Reset length
    	    static_cast<Derived *>(this)->len = 0;
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
        	if (!static_cast<Derived *>(this)->data)
            	static_cast<Derived *>(this)->Init(); // Lazy initialization

        	const auto required = static_cast<Trait::VeryLarge>(static_cast<Derived *>(this)->len + cap);
        	if (required <= static_cast<Derived *>(this)->capacity)
            	return; // Already enough capacity

        	auto new_cap = static_cast<Derived *>(this)->capacity;

        	// Grow until we reach the required capacity
        	while (new_cap < required)
        	{
            	new_cap = static_cast<Trait::VeryLarge>(new_cap * GrowthFactor);
        	}

	        // Resize to the new capacity
    	    static_cast<Derived *>(this)->Resize(new_cap);
    	}

    	/**
     	* @brief Construct an element at the end of the vector by copying \p value.
     	*
     	* If necessary, the vector will grow using the configured growth factor.
     	*
     	* @param args Arguments forwarded to T's constructor.
     	*/
    	template<typename... Args>
    	void EmplaceBack(Args &&...args)
    	{
        	// Resize if necessary
        	EnsureGrowth(1);

        	// Construct the new element in place
        	new (
				&static_cast<Derived *>(this)->data[
					static_cast<Derived *>(this)->len
				]
			) T(std::forward<Args>(args)...);
        	++static_cast<Derived *>(this)->len;
    	}

	    /**
    	 * @brief Push an element to the back of the vector.
     	 *
     	 * This forwards to EmplaceBack using move semantics.
     	 *
     	 * @param value Rvalue reference to the element to push.
     	 */
    	template<class U = T, typename = Trait::EnsureSame<T, U>>
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
            	static_cast<Derived *>(this)->data[
					static_cast<Derived *>(this)->len
				] = std::forward<U>(value);
            	++static_cast<Derived *>(this)->len;
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
        	if (static_cast<Derived *>(this)->len == 0)
        	{
            	throw Except::OutOfRange();
        	}

        	// Decrease length
        	--static_cast<Derived *>(this)->len;

        	// Only call destructor if T is not trivially destructible
        	if constexpr (!std::is_trivially_destructible_v<T>)
        	{
            	static_cast<Derived *>(this)->data[static_cast<Derived *>(this)->len].~T();
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
        	if (static_cast<Derived *>(this)->len == 0)
        	{
            	throw Except::OutOfRange();
        	}

        	// Decrease length
        	--static_cast<Derived *>(this)->len;

        	// Return the element by reference for move semantics
        	return static_cast<Derived *>(this)->data[
				static_cast<Derived *>(this)->len
			];
    	}

		void ResizeFill(const Trait::VeryLarge new_size, const T &value)
        {
            static_cast<Derived *>(this)->Resize(new_size);
			for (
				Trait::VeryLarge i = static_cast<Derived *>(this)->len;
				i < new_size;
				++i
			)
            {
                new (&static_cast<Derived *>(this)->data[i]) T(value);
            }

            static_cast<Derived *>(this)->len = new_size;
        }
	};

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
            typename = Trait::EnsureArrayAllocator<Allocator>
        >
        class Vector :
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
			friend class BufferedBase<Vector<T, GrowthFactor, InitialCapacity, Allocator>, T, GrowthFactor>;

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
                this->len = 0;
            }

            /**
             * @brief Construct the vector from an initializer list.
             *
             * Each element in \p init_list is copied/emplaced into the vector.
             *
             * @param init_list Initial values to insert.
             */
            template<bool IsConstruct = false>
            void ConstructFrom(std::initializer_list<T> init_list)
            {
                this->Clear(); // Clear current contents

                // Insert each element from the initializer list
                for (const auto &elem : init_list)
                {
                    this->EmplaceBack(elem);
                }
            }

            /**
             * @brief Construct the vector as a copy of another vector.
             *
             * Allocates sufficient storage and copies each element from \p other.
             *
             * @param other Source vector to copy from.
             */
            template<bool IsConstruct = false>
            void ConstructFrom(const Vector &other)
            {
                // Only check self-assignment for assignment operations
                if constexpr (!IsConstruct)
                {
                    if (this == &other) return; // Self-assignment check
                    this->Clear(); // Clear current contents
                }

                this->len = other.len;
                this->capacity = other.capacity;
                this->data = Allocator::Allocate(capacity);

                Utility::Copy(other.data, this->data, this->len);
            }

            /**
             * @brief Construct the vector by moving resources from another vector.
             *
             * Transfers ownership of resources from \p other to this vector,
             * leaving \p other in a valid but empty state.
             *
             * @param other Source vector to move from.
             */
            template<bool IsConstruct = false>
            void ConstructFrom(Vector &&other)
            {
                // Only check self-assignment for assignment operations
                if constexpr (!IsConstruct)
                {
                    if (this == &other) return; // Self-assignment check
                    this->Clear(); // Clear current contents
                }

                this->len = other.len;
                this->capacity = other.capacity;
                this->data = other.data;

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

        	Vector& operator=(const Vector &other) noexcept
        	{
        		if (this != &other)
        			ConstructFrom(other);
        		return *this;
        	}

        	Vector &operator=(Vector &&other) noexcept
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
            Vector() :
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
        	Vector(std::initializer_list<T> init_list) :
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
        	Vector(const Vector &other) :
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
        	Vector(Vector &&other) :
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

                    // Destroy old elements before freeing
                    if constexpr (!std::is_trivially_destructible_v<T>)
                        for (Trait::VeryLarge i = 0; i < this->len; ++i)
                            this->data[i].~T();
                }

                // Deallocate old data
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
            ~Vector() override
            {
                if (!this->data) return; // Nothing to do
                // Call the destructor for each element
                this->Clear();

                // Deallocate the data
                Allocator::Deallocate(this->data);
                this->data = nullptr;
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
