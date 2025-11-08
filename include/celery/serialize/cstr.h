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
     * @brief Specialization of Display for C-style strings (char*).
     *
     * This specialization provides methods to output C-style strings
     * in raw form to an output stream.
     */
    template<>
    struct Display<char *>
    {
        /*
         *  @brief Output a raw representation of a C-style string to the provided stream.
         *
         * @param obj The C-style string to output.
         * @param stream The output stream to write to.
         */
        static inline void Raw(char *&&obj, Io::Pmr::OStream<> &stream)
        {
            // Write the C-style string to the stream
            const char *cstr = static_cast<const char *>(obj);
            const auto len = strlen(cstr);
            stream.Batch(cstr, len);
        }
    };
}