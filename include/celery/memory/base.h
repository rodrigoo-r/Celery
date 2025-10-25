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
#include "celery/except/unimplemented.h"

namespace Celery::Pmr
{
    /**
     * @brief Polymorphic allocator interface for single objects.
     *
     * @tparam T Type of object to allocate.
     *
     * This class provides static allocation and deallocation entry points
     * that are expected to be supplied by a concrete memory resource
     * implementation. The default implementations throw
     * Except::UnimplementedException to indicate they must be overridden
     * by the user or a platform-specific allocator.
     */
    template <typename T>
    class Allocator
    {
    public:
        /**
         * @brief Allocate and construct an object of type T.
         *
         * @tparam Args Variadic template parameters forwarded to T's constructor.
         * @param args Arguments forwarded to T's constructor.
         * @return Pointer to the newly allocated and constructed T.
         *
         * @note The default implementation throws Except::UnimplementedException.
         *       Implementations should ensure proper construction and ownership
         *       semantics for the returned pointer.
         */
        template <typename ...Args>
        static inline T *Allocate(Args &&...args)
        {
            // Implementation should be user-defined
            throw Except::UnimplementedException();
        }

        /**
         * @brief Destroy and deallocate an object previously allocated with Allocate.
         *
         * @param ptr Pointer to the object to destroy and deallocate.
         *
         * @note The default implementation throws Except::UnimplementedException.
         *       Implementations must properly call the destructor of T and free
         *       the underlying memory.
         */
        static void Deallocate(T *ptr)
        {
            // Implementation should be user-defined
            throw Except::UnimplementedException();
        }
    };

    /**
     * @brief Allocator interface for arrays of objects.
     *
     * @tparam T Element type of the array to allocate.
     *
     * Provides static methods for allocating and deallocating arrays.
     * The default implementations throw Except::UnimplementedException and
     * should be replaced by concrete implementations that handle array
     * construction and destruction correctly.
     */
    template <typename T>
    class ArrayAllocator
    {
    public:
        /**
         * @brief Allocate an array of `size` elements of type T.
         *
         * @param size Number of elements to allocate.
         * @return Pointer to the first element of the allocated array.
         *
         * @note Implementations should construct each element as appropriate.
         *       The default implementation throws Except::UnimplementedException.
         */
        static inline T *Allocate(size_t size)
        {
            // Implementation should be user-defined
            throw Except::UnimplementedException();
        }

        /**
         * @brief Destroy and deallocate an array previously allocated with Allocate.
         *
         * @param ptr Pointer to the first element of the array to destroy and deallocate.
         *
         * @note Implementations must correctly call destructors for each element
         *       and free the underlying memory. The default implementation throws
         *       Except::UnimplementedException.
         */
        static void Deallocate(T *ptr)
        {
            // Implementation should be user-defined
            throw Except::UnimplementedException();
        }
    };
}