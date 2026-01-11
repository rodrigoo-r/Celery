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
#include "Celery/Array/External.h"

namespace Celery::Str
{
    class External : public Array::External<char>
    {
    public:
        using Array::External<char>::External;

		/**
         * @brief Construct an External view from a string literal.
         *
         * @tparam N Size of the string literal including null terminator.
         * @param literal The string literal to create a view for.
         *
         * Behavior & complexity:
         * - O(1). No copying is performed.
         * - The view represents the characters in the literal excluding the
         *   terminating null character.
         */
		template <std::size_t N>
		External(const char (&literal)[N])
		{
    		this->data = const_cast<char*>(literal);
    		this->len  = N - 1; // exclude null terminator
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
    };
}
