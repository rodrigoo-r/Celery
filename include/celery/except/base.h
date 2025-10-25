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
#include <exception>

namespace Celery::Except
{
    class Exception : public std::exception
    {
        const char *msg;

    public:
        Exception(const char *msg) : msg(msg)
        {}

        [[nodiscard]] const char *what()
        const noexcept override
        {
            return msg;
        }
    };
}