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
#include "base.h"
#include "celery/except/bad_alloc.h"

namespace Celery::Pmr
{
    template <typename T>
    class SystemAllocator : Allocator<T>
    {
    public:
        /**
         * @brief Allocate raw memory for a single object of type T and construct it.
         *
         * Allocates exactly sizeof(T) bytes using the global ::operator new and then
         * constructs an instance of T in-place using placement new with the provided
         * constructor arguments.
         *
         * @tparam Args Variadic template parameter pack for constructor arguments.
         * @param args Constructor arguments forwarded to T's constructor.
         * @return Pointer to the newly-constructed T instance.
         */
        template <typename ...Args>
        static inline T *Allocate(Args &&...args)
        {
            // Create a raw ptr
            T *ptr = static_cast<T *>(::operator new(sizeof(T)));
            if (ptr == nullptr)
            {
                throw Except::BadAlloc(); // Allocation failed
            }

            // Use placement new to construct the object
            new (ptr) T(std::forward<Args>(args)...);

            // Return the constructed object
            return ptr;
        }

        /**
         * @brief Destroy and deallocate an object created by Allocate.
         *
         * If T is not trivially destructible, explicitly calls the destructor.
         * Afterwards releases the memory using the global ::operator delete. Passing
         * a null pointer is safe (operator delete accepts null).
         *
         * @param ptr Pointer to the object to be destroyed and deallocated.
         */
        static inline void Deallocate(T *ptr)
        {
            // Only call destructor if not trivially destructible
            if constexpr (!std::is_trivially_destructible_v<T>)
            {
                ptr->~T(); // Call the destructor
            }

            ::operator delete(ptr); // Free the memory
        }
    };

    template <typename T>
    class SystemArrayAllocator : ArrayAllocator<T>
    {
    public:
        /**
         * @brief Allocate raw memory for an array of T objects.
         *
         * Allocates enough uninitialized memory to hold \p count objects of type T
         * using the global ::operator new. This function does not construct the
         * individual objects; callers must construct elements using placement new.
         *
         * @param count Number of T objects to allocate memory for.
         * @return Pointer to uninitialized memory capable of holding \p count T objects.
         * @throws std::bad_alloc if allocation fails.
         */
        static inline T *Allocate(const size_t count)
        {
            // Allocate raw memory for count T objects
            T *ptr = static_cast<T *>(::operator new(sizeof(T) * count));
            if (ptr == nullptr)
            {
                throw Except::BadAlloc(); // Allocation failed
            }

            return ptr;
        }

        /**
         * @brief Deallocate memory previously obtained from Allocate.
         *
         * Releases memory allocated by Allocate using the global ::operator delete.
         * This function does not call destructors for objects stored in the memory;
         * callers must explicitly destroy objects (e.g., by calling their destructors)
         * before invoking Deallocate. Passing a null pointer is safe.
         *
         * @param ptr Pointer to memory returned by Allocate (may be nullptr).
         */
        static inline void Deallocate(T *ptr)
        {
            ::operator delete(ptr);
        }
    };
}