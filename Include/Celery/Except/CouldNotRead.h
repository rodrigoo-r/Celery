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
#include "Base.h"

/**
 * @namespace Celery::Except
 * @brief Contains exception types used across the Celery library.
 *
 * Exceptions in this namespace provide consistent error reporting
 * for various error conditions encountered by Celery components.
 */
namespace Celery::Except
{
    /**
     * @class CouldNotRead
     * @brief Exception thrown when a file or socket cannot be read.
     *
     * This exception indicates that an attempt to read from a file
     * or socket has failed, possibly due to I/O errors, permission
     * issues, or other underlying problems.
     */
    class CouldNotRead : public Exception
    {
    public:
        /**
         * @brief Constructs a CouldNotRead exception with a default message.
         */
        CouldNotRead()
            : Exception("Could not read a file or socket.")
        {}

        CouldNotRead(Celery::Str::External &details)
            : Exception(details)
        {}
    };
}