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

namespace Celery::Base
{
    /**
     * @brief CRTP mixin that provides pointer-like access to a managed object.
     *
     * This class uses the Curiously Recurring Template Pattern (CRTP) to
     * provide dereference (`operator*`) and arrow (`operator->`) operators
     * that forward to the derived class's `data` member.
     *
     * @tparam Derived The final type that inherits from Ptr\<Derived, T\>.
     *         Must have a member `T* data` pointing to the managed object.
     * @tparam T The type of the managed object.
     */
    template <typename Derived, typename T>
    class Dereferenceable
    {
    public:
        /**
         * @brief Dereference operator to access the managed object.
         *
         * @returns Reference to the managed object.
         */
        T &operator*() const
        {
            return *static_cast<Derived*>(this)->data;
        }

        /**
         * @brief Arrow operator to access members of the managed object.
         *
         * @returns Pointer to the managed object.
         */
        T *operator->() const
        {
            return static_cast<Derived*>(this)->data;
        }
    };
}