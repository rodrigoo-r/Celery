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

/**
 * @namespace Celery::Except
 * @brief Exception types used across the Celery library.
 *
 * This namespace contains lightweight exception types that mirror
 * the behavior of standard C++ exceptions but are specific to
 * Celery's error reporting.
 */
namespace Celery::Except
{
    /**
     * @class Exception
     * @brief A minimal exception wrapper around a C-style message.
     *
     * This class derives from std::exception and holds a pointer to a
     * null-terminated C string describing the error. The message pointer
     * is not copied by this class; callers must ensure the lifetime of
     * the provided string outlives the exception object.
     */
    class Exception : public std::exception
    {
        Celery::Str::External msg;

    public:
        /**
         * @brief Construct an Exception with a message.
         * @param msg A null-terminated C string describing the error.
         *
         * Note: The string is not owned by this object. Keep the string
         * valid for the lifetime of the exception.
         */
        Exception(Celery::Str::External &msg) : msg(msg)
        {}

        /**
         * @brief Return the explanatory string.
         * @return The message provided at construction.
         *
         * This overrides std::exception::what() and is guaranteed to be
         * noexcept.
         */
        [[nodiscard]] const char *what()
        const noexcept override
        {
            return msg.Ptr();
        }
    };
}