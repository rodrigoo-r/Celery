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
#include "celery/base/indexable.h"

namespace Celery::Str
{
    class External : public Base::Indexable<char>
    {
    public:
        template <
            typename T,
            typename = std::enable_if_t<
                std::is_pointer_v<std::decay_t<T>> &&
                std::is_same_v<
                    std::remove_cv_t<std::remove_pointer_t<std::decay_t<T>>>,
                    char
                >
            >
        >
        External(T &&data_ptr, const Trait::VeryLarge length)
        {
            this->data = const_cast<char *>(data_ptr);
            this->len = length;
        }

        template <
            typename T,
            typename = std::enable_if_t<
                std::is_pointer_v<std::decay_t<T>> &&
                std::is_same_v<
                    std::remove_cv_t<std::remove_pointer_t<std::decay_t<T>>>,
                    char
                >
            >
        >
        External(T &&data_ptr)
        {
            this->data = const_cast<char *>(data_ptr);
            this->len = strlen(this->data);
        }
    };
}