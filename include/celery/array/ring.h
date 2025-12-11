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
#include <cstring>


#include "celery/base/iterable.h"
#include "celery/base/pushable.h"
#include "celery/base/removable.h"
#include "celery/base/sizeable.h"
#include "celery/except/out_of_range.h"
#include "celery/memory/system.h"
#include "celery/trait/type.h"

namespace Celery::Buffer
{
    namespace Pmr
    {
        /**
         * @brief Fixed-capacity ring buffer (circular buffer) template.
         *
         * The buffer stores up to \p Capacity elements of type \p T. If
         * \p UseHeap is true, the storage is allocated on the heap using the
         * provided \p Allocator; otherwise the storage is placed inline as a
         * C-style array of size \p Capacity.
         *
         * Template parameters:
         * @tparam T Element type stored in the buffer.
         * @tparam Capacity Maximum number of elements the buffer can hold.
         * @tparam UseHeap If true, use heap allocation for storage.
         * @tparam Allocator Allocator type used when \p UseHeap is true.
         *
         * Requirements:
         * - \p Capacity must be > 0 (enforced via SFINAE).
         * - \p Allocator must derive from Pmr::ArrayAllocator<T> (enforced via SFINAE).
         */
        template<
            typename T,
            unsigned int Capacity = 50,
            bool UseHeap = Capacity >= 256,
            typename Allocator = Celery::Pmr::ArrayAllocator<T>,
            // SFINAE to ensure Capacity > 0
            typename = std::enable_if_t<(Capacity > 0)>,
            // SFINAE to ensure Allocator is valid
            typename = Trait::EnsureArrayAllocator<Allocator>
        >
        class Ring :
            public Base::Sizeable,
            public Base::Pushable<Ring<T, Capacity, UseHeap, Allocator>, T>,
            public Base::BufferedRemovable<Ring<T, Capacity, UseHeap, Allocator>, T>,
            public Base::BufferedIterable<Ring<T, Capacity, UseHeap, Allocator>, T>
        {
            // Make Base::BufferedRemovable and Base::BufferedIterable friends to access protected members
            friend class Base::BufferedRemovable<
                Ring<T, Capacity, UseHeap, Allocator>,
                T
            >;
            friend class Base::BufferedIterable<
                Ring<T, Capacity, UseHeap, Allocator>,
                T
            >;

            // Data storage type
            using DataType = std::conditional_t<UseHeap, T *, T[Capacity]>;

            /**
             * @brief Underlying storage for the ring buffer.
             *
             * When \p UseHeap is true, this is a pointer returned by \p Allocator::Allocate.
             * When \p UseHeap is false, this is an inline array of \p Capacity elements.
             */
            DataType data;

        protected:
            /**
             * @brief Handle a batch of elements from the buffer.
             *
             * Pure virtual hook invoked to process or consume up to \p count elements
             * starting at \p data. Implementations must define how the batch is handled
             * (e.g. forwarding, processing, or transferring ownership).
             *
             * @note \p data points into the ring buffer's internal storage and is valid
             *       for at least \p count elements. Do not assume null-termination or
             *       lifetime beyond the caller's contract.
             *
             * @param data Pointer to the first element of the batch.
             * @param count Number of elements available at \p data.
             */
            virtual void HandleBatch(T *data, size_t count) = 0;

            /**
             * @brief Construct the ring buffer from an initializer list.
             *
             * Each element in \p init_list is copied/emplaced into the buffer.
             *
             * @param init_list Initial values to insert.
             */
            template <bool IsConstruct = false>
            void ConstructFrom(std::initializer_list<T> init_list)
            {
                if constexpr (UseHeap)
                {
                    if (!data)
                    {
                        data = Allocator::Allocate(Capacity);
                    }
                }

                if constexpr (!IsConstruct)
                {
                    Clear(); // Clear current contents
                }

                // Insert each element from the initializer list
                for (const auto &elem : init_list)
                {
                    EmplaceBack(elem);
                }
            }

            /**
             * @brief Construct the ring buffer as a copy of another ring buffer.
             *
             * Allocates storage if needed and copies elements from \p other
             * into this buffer using the \p Batch method.
             *
             * @param other Source ring buffer to copy from.
             */
            template<bool IsConstruct = false>
            void ConstructFrom(const Ring &other)
            {
                // Only check self-assignment for assignment operations
                if constexpr (!IsConstruct)
                {
                    if (this == &other) return; // Self-assignment check
                }

                if (this->len != 0)
                {
                    Clear(); // Clear current contents
                }
                else
                {
                    // Allocate heap storage if needed
                    if constexpr (UseHeap)
                    {
                        data = Allocator::Allocate(Capacity);
                    }
                }

                // Copy elements from other
                Batch(other.data, other.len);
            }

            /**
             * @brief Construct the ring buffer by moving resources from another ring buffer.
             *
             * Transfers ownership of resources from \p other to this buffer,
             * leaving \p other in a valid but empty state.
             *
             * @param other Source ring buffer to move from.
             */
            template<bool IsConstruct = false>
            void ConstructFrom(Ring &&other)
            {
                // Only check self-assignment for assignment operations
                if constexpr (!IsConstruct)
                {
                    if (this == &other) return; // Self-assignment check
                    Clear(); // Clear current contents
                }

                if constexpr (UseHeap)
                {
                    data = other.data;
                    other.data = nullptr;
                }
                else
                {
                    // Move inline data
                    for (size_t i = 0; i < other.len; ++i)
                    {
                        data[i] = std::move(other.data[i]);
                    }
                }
                len = other.len;
                other.len = 0;
            }

        public:
            /**
             * @brief Construct a new RingBuffer.
             *
             * If heap storage is selected via the template parameter \p UseHeap,
             * the constructor will allocate storage using \p Allocator::Allocate.
             */
            Ring() : Sizeable()
            {
                // Allocate heap storage if needed
                if constexpr (UseHeap)
                {
                    data = Allocator::Allocate(Capacity);
                }
            }

            /**
             * @brief Construct the ring buffer from an initializer list.
             *
             * Each element in \p init_list is copied/emplaced into the buffer.
             *
             * @param init_list Initial values to insert.
             */
            Ring(std::initializer_list<T> init_list)
            {
                ConstructFrom(init_list);
            }

            /**
             * @brief Copy constructor.
             *
             * Allocates storage if needed and copies elements from \p other
             * into this buffer using the \p Batch method.
             *
             * @param other The Ring buffer to copy from.
             */
            Ring(const Ring &other)
            {
                ConstructFrom<true>(other);
            }

            /**
             * @brief Move constructor.
             *
             * Transfers ownership of resources from \p other to this buffer.
             * If heap storage is used, the internal pointer is moved and
             * \p other's pointer is set to nullptr. For inline storage,
             * elements are moved individually.
             *
             * @param other The Ring buffer to move from.
             */
            Ring(Ring &&other) noexcept
            {
                ConstructFrom<true>(other);
            }

            /**
             * @brief Copy assignment operator.
             *
             * Clears current contents and copies elements from \p other
             * into this buffer using the \p Batch method.
             *
             * @param other The Ring buffer to copy from.
             * @return Reference to this Ring buffer.
             */
            Ring &operator=(const Ring &other)
            {
                ConstructFrom(other);
                return *this;
            }

            /**
             * @brief Move assignment operator.
             *
             * Clears current contents and transfers ownership of resources
             * from \p other to this buffer.
             *
             * @param other The Ring buffer to move from.
             * @return Reference to this Ring buffer.
             */
            Ring &operator=(Ring &&other) noexcept
            {
                ConstructFrom(other);
                return *this;
            }

            /**
             * @brief Return the current head index (number of elements stored).
             *
             * @return size_t Current occupied length of the buffer.
             */
            [[nodiscard]] size_t Head() const noexcept
            {
                return len;
            }

            /**
             * @brief Construct an element in-place at the back of the ring buffer.
             *
             * If the buffer is full, this will wrap around and overwrite from the beginning.
             * For trivially copyable types, assignment is used. For non-trivial types,
             * placement new is used to construct the object.
             *
             * @tparam Args Argument pack forwarded to \p T constructor.
             * @param args Constructor arguments for the new element.
             */
            template<typename... Args>
            void EmplaceBack(Args &&...args)
            {
                if (len >= Capacity)
                {
                    // Handle full buffer
                    HandleBatch(data, len);

                    // Wrap around
                    len = 0;
                }

                // Write in the next position
                if constexpr (std::is_trivially_copyable_v<T>)
                {
                    // Construct the object directly for trivially copyable types
                    data[len++] = T(std::forward<Args>(args)...);
                }
                else
                {
                    // Use placement new for non-trivially copyable types
                    new (&data[len++]) T(std::forward<Args>(args)...);
                }
            }

            /**
             * @brief Write a value into the buffer by forwarding to emplace.
             *
             * This is a convenience wrapper that forwards to the emplacing routine.
             *
             * @tparam U Type of the value being written.
             * @param val Value to write into the buffer.
             */
            template<
                class U,
                typename = Trait::EnsureSame<T, U>
            >
            void Write(U &&val)
            {
                // Forward to emplace_back
                EmplaceBack(val);
            }

            /**
             * @brief Batch-insert elements into the ring buffer.
             *
             * Copies up to \p count elements from the provided \p data array by calling
             * EmplaceBack for each element. This will follow the ring buffer's wrap-around
             * and overwrite behavior when the capacity is exceeded.
             *
             * @param ptr Pointer to the source elements (must point to at least \p count elements).
             * @param count Number of elements to insert from \p data.
             */
            void Batch(const T *ptr, const size_t count)
            {
                // Use memset if trivially copyable
                if constexpr (std::is_trivially_copyable_v<T>)
                {
                    // Define the remaining count
                    size_t remaining = count;

                    // Copy what fits until we run out of space
                    while (remaining != 0)
                    {
                        // Get what fits
                        auto available = Capacity - len;
                        if (available == 0)
                        {
                            // Handle full buffer
                            HandleBatch(data, len);
                            len = 0; // Wrap around
                            available = len;
                        }

                        const auto to_copy = (remaining < available) ? remaining : available;
                        memcpy(&data[len], ptr + (count - remaining), sizeof(T) * to_copy);
                        len += to_copy;
                        remaining -= to_copy;
                    }
                }
                else
                {
                    // Emplace each element individually
                    for (size_t i = 0; i < count; ++i)
                    {
                        EmplaceBack(ptr[i]);
                    }
                }
            }

            /**
             * @brief Access element by index.
             *
             * Performs bounds checking against the current length and throws
             * Except::OutOfRangeException on invalid index.
             *
             * @param index Index of the element to access (0-based).
             * @return T& Reference to the element at \p index.
             * @throws Except::OutOfRangeException if index >= Head().
             */
            T &operator[](const size_t index)
            {
                if (index >= len)
                {
                    throw Except::OutOfRange();
                }

                return data[index];
            }

            /**
             * @brief Check whether the buffer is full.
             *
             * @return true if the number of elements equals \p Capacity.
             */
            [[nodiscard]] bool Full() const { return len == Capacity; }

            /**
             * @brief Remove the last element from the buffer.
             *
             * Decrements the stored length. If the buffer is empty, throws
             * Except::OutOfRangeException.
             *
             * Note: this does not call the destructor for trivially copyable
             * types; for non-trivial types the storage will remain but the
             * logical length is decremented.
             *
             * @throws Except::OutOfRangeException if the buffer is empty.
             */
            void PopBack()
            {
                if (len == 0)
                {
                    throw Except::OutOfRange();
                }

                // Decrement length
                --len;
            }

            /**
             * @brief Clear the buffer logically.
             *
             * Resets the stored length to zero. Does not deallocate storage or
             * call destructors for contained objects.
             */
            void Clear()
            {
                // Call destructors if necessary
                if constexpr (!std::is_trivially_destructible_v<T>)
                {
                    for (size_t i = 0; i < len; ++i)
                    {
                        data[i].~T();
                    }
                }

                // Reset length to zero
                len = 0;
            }

            /**
             * @brief Get raw pointer to the underlying storage.
             *
             * Returns a pointer to the internal buffer storage. For heap-backed buffers
             * this returns the allocated pointer; for inline storage it returns a pointer
             * to the first element. The returned pointer provides direct access to raw
             * storage (no bounds checking). It may be invalidated when the buffer is
             * destroyed or when operations that reallocate occur.
             *
             * @return T* Pointer to the first element of the buffer.
             */
            [[nodiscard]] T *Ptr() const noexcept
            {
                return data;
            }

            /**
             * @brief Destroy the ring buffer.
             *
             * If heap storage was used, deallocates the storage using
             * \p Allocator::Deallocate.
             */
            ~Ring() override
            {
                // Call destructors if necessary
                if constexpr (!std::is_trivially_destructible_v<T>)
                {
                    for (size_t i = 0; i < len; ++i)
                    {
                        data[i].~T();
                    }
                }

                // Deallocate heap storage if used
                if constexpr (UseHeap)
                {
                    Allocator::Deallocate(data);
                }
            }
        };

        /**
         * @brief Wrap-around ring buffer that forwards full batches to HandleBatch.
         *
         * Inherits from Ring and exposes the same interface. When the buffer
         * becomes full, it automatically calls \p HandleBatch to process the
         * current contents before wrapping around.
         *
         * @tparam T Element type stored in the buffer.
         * @tparam Capacity Maximum number of elements the buffer can hold.
         * @tparam UseHeap If true, use heap allocation for storage.
         * @tparam Allocator Allocator type used when \p UseHeap is true.
         */
        template<
            typename T,
            unsigned int Capacity = 50,
            bool UseHeap = Capacity >= 256,
            typename Allocator = Celery::Pmr::ArrayAllocator<T>
        >
        class WrapAroundRing : public Ring<T, Capacity, UseHeap, Allocator>
        {
        protected:
            void HandleBatch(T *data, size_t count) override
            {
                // Default implementation does nothing.
                // Users should override this method to define custom behavior.
            }

        public:
            using Ring<T, Capacity, UseHeap, Allocator>::Ring;
        }
    } // namespace Pmr

    /**
     * @brief Convenience alias for the PMR ring buffer with default allocator decisions.
     *
     * @tparam T Element type.
     * @tparam Capacity Buffer capacity (default 50).
     */
    template<typename T, unsigned int Capacity = 50>
    using RingBuffer = Pmr::Ring<T, Capacity>;
} // namespace Celery::Buffer
