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
#include "celery/string/external.h"
#include "displayable.h"

namespace Celery::Serialize
{
    template<>
    struct Display<Str::External>
    {
        /*
         *  @brief Output a raw representation of a Celery::Str::External to the provided stream.
         *
         * @param obj The Celery::Str::External to output.
         * @param stream The output stream to write to.
         */
        template<
            class U = Str::External,
            typename = Trait::EnsureSame<U, Str::External>
        >
        static inline void Raw(U &&obj, Io::Pmr::OStream<> &stream)
        {
            stream.Batch(obj.Ptr(), obj.Len());
        }
    };
}