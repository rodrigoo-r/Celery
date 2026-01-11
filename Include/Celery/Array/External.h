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

#include "Celery/Base/Indexable.h"
#include "Celery/Trait/Default.h"

namespace Celery::Array 
{
    template <typename Element>
    class External :
        public Base::Indexable<Element>
    {
    public:
        /**
         * @brief Construct an External view from a pointer and explicit length.
         *
         * Template constraints:
         * - `T` must decay to a pointer type.
         * - The pointed-to type (after removing cv and pointer) must be `char`.
         *
         * @tparam T Pointer type convertible to `char*` or `const char*`.
         * @param data_ptr Pointer to the first character of the external buffer.
         *                 Must not be `nullptr`.
         * @param length Number of characters in the buffer (may be large).
         *
         * Behavior & complexity:
         * - O(1). No copying is performed.
         * - Does not take ownership; the caller must ensure `data_ptr` remains
         *   valid for the lifetime of this object.
         */
        template <
            typename T,
            typename = std::enable_if_t<
                std::is_pointer_v<std::decay_t<T>> &&
                std::is_same_v<
                    std::remove_cv_t<std::remove_pointer_t<std::decay_t<T>>>,
                    Element
                >
            >
        >
        External(
            T &&data_ptr,
            const Celery::Trait::VeryLarge length
        )
        {
            // compute length by scanning for the null terminator
            this->data = const_cast<Element *>(data_ptr);
            this->len = length;
        }

        /**
         * @brief Default constructor creates an empty External view.
         *
         * Behavior & complexity:
         * - O(1). No copying is performed.
         * - The view represents an empty sequence (length 0).
         */
        External()
        {
            // Empty view
            this->data = nullptr;
            this->len = 0;
        }

        /**
         * @brief Default copy constructor.
         *
         * Behavior & complexity:
         * - O(1). No copying of the underlying data is performed.
         */
		External(const External &other) = default;
        External(External &&other) noexcept = default;

        /**
         * @brief Default copy assignment operator.
         *
         * Behavior & complexity:
         * - O(1). No copying of the underlying data is performed.
         */
        External &operator=(const External &other) = default;
        External &operator=(External &&other) noexcept = default;

        /**
         * @brief Equality operator to compare two External views.
         *
         * Compares the size and content of the two views for equality.
         *
         * @param other The other External view to compare against.
         * @return true if both views have the same size and identical content.
         * @return false otherwise.
         *
         * Behavior & complexity:
         * - O(n) where n is the size of the views, due to `memcmp`.
         */
        bool operator==(const External &other) const noexcept
        {
            return this->Size() == other.Size() &&
                   memcmp(this->Ptr(), other.Ptr(), this->Size()) == 0;
        }

        /**
         * @brief Inequality operator to compare two External views.
         *
         * @param other The other External view to compare against.
         * @return true if the views are not equal.
         * @return false if the views are equal.
         *
         * Behavior & complexity:
         * - O(n) where n is the size of the views, due to `operator==`.
         */
        bool operator!=(const External &other) const noexcept
        {
            return !(*this == other);
        }
    };
}