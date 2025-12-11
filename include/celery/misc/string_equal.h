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

#include <celery/trait/default.h>
#include <celery/trait/type.h>
#include <celery/string/string.h>
#include <celery/string/external.h>

namespace Celery::Misc
{
    struct StringEquality
    {
        using is_transparent = void;

        template <
            typename T,
            typename U,
            typename = std::enable_if<
                Trait::EnsureSameBase<
                    T, Celery::Str::String
                > ||
                Trait::EnsureSameBase<
                    T, Celery::Str::External
                >
            >,
            typename = std::enable_if<
                Trait::EnsureSameBase<
                    U, Celery::Str::String
                > ||
                Trait::EnsureSameBase<
                    U, Celery::Str::External
                >
            >
        >
        static bool Cmp(T &&a, U &&b)
        {
            return a.Size() == b.Size() &&
                   memcmp(a.Ptr(), b.Ptr(), a.Size()) == 0;
        }

		template <
            typename T,
            typename U
		>
		bool operator()(T &&a, U &&b) const noexcept
		{
			return Cmp(
                std::forward<T>(a),
                std::forward<U>(b)
            );
		}
    };
}