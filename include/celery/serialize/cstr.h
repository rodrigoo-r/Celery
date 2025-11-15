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
#include "displayable.h"

namespace Celery::Serialize
{
    /**
     * @brief Specialization of Display for C-style strings (char* and const char*).
     *
     * This specialization provides methods to output C-style strings
     * in raw form to the provided output stream.
     */
    template<typename CharPtr>
        requires
            std::same_as<std::remove_cvref_t<CharPtr>, char*>
            || std::same_as<std::remove_cvref_t<CharPtr>, const char*>
    struct Display<CharPtr>
    {
        /**
         * @brief Output a raw representation of a C-style string to the provided stream.
         *
         * @param obj The C-style string to output.
         * @param stream The output stream to write to.
         */
        template<
            class U = CharPtr,
            typename = Trait::EnsureSame<U, CharPtr>
        >
        static inline void Raw(U &&obj, Io::Pmr::OStream<> &stream)
        {
            const char *cstr = static_cast<const char *>(obj);
            const auto len = strlen(cstr);
            stream.Batch(cstr, len);
        }
    };

    /**
     * @brief Specialization of Display for fixed-size C-style string arrays.
     *
     * This specialization handles arrays of characters with a known size,
     * outputting the string content (excluding the null terminator) to the stream.
     *
     * @tparam N Size of the character array, including the null terminator.
     */
    template<std::size_t N>
    struct Display<const char[N]>
    {
        /**
         * @brief Output a raw representation of a fixed-size C-style string array to the provided stream.
         *
         * @param obj The fixed-size C-style string array to output.
         * @param stream The output stream to write to.
         */
        static inline void Raw(const char (&obj)[N], Io::Pmr::OStream<> &stream)
        {
            // N includes the null terminator
            stream.Batch(obj, N - 1);
        }
    };
}