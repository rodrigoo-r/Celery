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
#include "celery/base/indexable.h"

namespace Celery::Str
{
    /**
     * @brief A non-owning view wrapper around a contiguous sequence of chars.
     *
     * External provides an indexable view into externally-managed character data.
     * It does NOT own the underlying storage; callers must ensure the data remains
     * valid for the lifetime of the External instance.
     *
     * Notes:
     * - Construction from a `const char*` is permitted, but the underlying pointer
     *   is stored as a non-const `char*` via `const_cast` to match
     *   `Base::Indexable<char>`'s API. Mutating the pointed memory through this
     *   view when the original storage is const yields undefined behavior.
     * - The class relies on the caller for lifetime management and does not
     *   perform allocations or copies of the data.
     */
    class External : public Base::Indexable<char>
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
                    char
                >
            >
        >
        External(T &&data_ptr, const Trait::VeryLarge length)
        {
            // store pointer (may remove constness). See class note regarding UB if mutated.
            this->data = const_cast<char *>(data_ptr);
            this->len = length;
        }

        /**
         * @brief Construct an External view from a null-terminated C string.
         *
         * Template constraints:
         * - `T` must decay to a pointer type.
         * - The pointed-to type (after removing cv and pointer) must be `char`.
         *
         * @tparam T Pointer type convertible to `char*` or `const char*`.
         * @param data_ptr Pointer to a null-terminated C string. Must not be `nullptr`.
         *
         * Behavior & complexity:
         * - Computes the length with `strlen`, so complexity is O(n) where `n` is
         *   the length of the C string (up to the terminating `\\0`).
         * - No copying is performed; the view does not own the storage.
         * - If `data_ptr` points to non-null-terminated memory, `strlen` is UB.
         */
        template <
            typename T,
            typename = std::enable_if_t<
                std::is_pointer_v<std::decay_t<T>> &&
                std::is_same_v<
                    std::remove_cv_t<std::remove_pointer_t<std::decay_t<T>>>,
                    char
                >
            >
        >
        External(T &&data_ptr)
        {
            // compute length by scanning for the null terminator
            this->data = const_cast<char *>(data_ptr);
            this->len = strlen(this->data);
        }

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
