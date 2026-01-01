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
     * @class BadRead
     * @brief Exception thrown when a stream read operation fails.
     *
     * This exception indicates that an attempt to read from a stream
     * was unsuccessful, typically due to end-of-file or an I/O error.
     */
    class BadRead : public Exception
    {
    public:
        /**
         * @brief Constructs a BadRead exception with a default error message.
         */
        BadRead()
            : Exception("Stream read error: Unable to read from the input source.")
        {}
    };
}