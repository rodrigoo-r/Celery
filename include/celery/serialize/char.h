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
     * @brief Specialization of Display for char type.
     *
     * This specialization provides methods to convert a char
     * to its string representation and to output it in raw form.
     */
    template<>
    struct Display<char>
    {
        /*
         *  @brief Convert a char to its string representation.
         *
         * @param obj The char to convert.
         * @return Str::String The string representation of the char.
         */
        static inline void Raw(char &&obj, Io::Pmr::OStream<> &stream)
        {
            // Write the char directly to the stream
            stream += obj;
        }
    };
}