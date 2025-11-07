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
     * @brief Specialization of Display for bool type.
     *
     * This specialization provides methods to convert a bool
     * to its string representation ("true" or "false") and
     * to output it in raw form.
     */
    template<>
    struct Display<bool>
    {
        /*
         *  @brief Output a raw representation of a bool to the provided stream.
         *
         * @param obj The bool to output.
         * @param stream The output stream to write to.
         */
        static inline void Raw(const bool &obj, Io::Pmr::OStream<> &stream)
        {
            // Boolean type
            const char *bool_str = obj ? "true" : "false";
            stream.Batch(bool_str, obj ? 4 : 5); // Length of "true" or "false"
        }
    };
}