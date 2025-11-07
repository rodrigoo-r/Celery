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
     * @brief Specialization of Display for floating-point types.
     *
     * This specialization provides methods to serialize floating-point types
     * to their string representation using Algorithm::Ftoa.
     *
     * @tparam T Floating-point type to specialize for.
     */
    template<std::floating_point T>
    struct Display<T>
    {
        /*
         *  @brief Output a raw representation of a floating-point type to the provided stream.
         *
         * @param obj The floating-point value to output.
         * @param stream The output stream to write to.
         */
        static inline void Raw(const T &obj, Io::Pmr::OStream<> &stream)
        {
            // Floating type
            Algorithm::Ftoa(obj, stream);
        }
    };
}