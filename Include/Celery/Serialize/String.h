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
#include "Displayable.h"

namespace Celery::Serialize
{
    /**
     * @brief Specialization of Display for Celery::Str::String type.
     *
     * This specialization provides methods to output Celery::Str::String
     * in raw form to an output stream.
     */
    template<>
    struct Display<Str::String>
    {
        /*
         *  @brief Output a raw representation of a Celery::Str::String to the provided stream.
         *
         * @param obj The Celery::Str::String to output.
         * @param stream The output stream to write to.
         */
        template<
            class U = Str::String,
            typename = Trait::EnsureSame<U, Str::String>
        >
        static inline void Raw(U &&obj, Io::Pmr::OStream<> &stream)
        {
            stream.Batch(obj.Ptr(), obj.Len());
        }
    };
}