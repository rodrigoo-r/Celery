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

namespace Celery::Pmr
{
    template <typename T>
    class SystemAllocator : Allocator<T>
    {
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
}