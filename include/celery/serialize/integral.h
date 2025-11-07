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
#include "celery/algo/ftoa.h"
#include "displayable.h"

namespace Celery::Serialize
{
    /**
     * @brief Specialization of Display for integral types.
     *
     * This specialization provides methods to serialize integral types
     * to their string representation using Algorithm::Itoa.
     *
     * @tparam T Integral type to specialize for.
     */
    template<std::integral T>
    struct Display<T>
    {
        /*
         *  @brief Output a raw representation of an integral type to the provided stream.
         *
         * @param obj The integral value to output.
         * @param stream The output stream to write to.
         */
        static inline void Raw(const T &obj, Io::Pmr::OStream<> &stream)
        {
            // Integral type
            Algorithm::Itoa(obj, stream);
        }
    };
}