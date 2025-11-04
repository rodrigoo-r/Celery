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
#include <utility>

namespace Celery::Base
{
    template <typename Derived, typename T>
    class Pushable {
    public:
        template <class U = T>
        Derived &operator+=(U &&value) {
            // Forward to the derived class's EmplaceBack method
            static_cast<Derived*>(this)->EmplaceBack(std::forward<U>(value));
            return static_cast<Derived&>(*this);
        }
    };
}