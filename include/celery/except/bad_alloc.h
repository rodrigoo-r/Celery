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
#include "base.h"

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
     * @class BadAlloc
     * @brief Exception type indicating memory allocation failure.
     *
     * Thrown when a memory allocation cannot be satisfied. This class
     * derives from `Exception` and provides a standard message describing
     * the allocation failure (similar in purpose to `std::bad_alloc`).
     */
    class BadAlloc : public Exception
    {
    public:
        /**
         * @brief Construct a new BadAlloc exception.
         *
         * Initializes the base `Exception` with a clear, descriptive message
         * that can be reported to callers or logged.
         */
        BadAlloc()
            : Exception("Memory allocation failed.")
        {}
    };
}