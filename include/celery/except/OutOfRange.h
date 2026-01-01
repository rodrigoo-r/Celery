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
     * @class OutOfRange
     * @brief Thrown when access is attempted outside a valid range.
     *
     * This exception represents range-related errors (for example, accessing
     * a container with an invalid index). It derives from Celery::Except::Exception
     * and currently provides a default descriptive message. Additional
     * constructors can be added to include contextual information.
     */
    class OutOfRange : public Exception
    {
    public:
        /**
         * @brief Construct a new OutOfRangeException with a default message.
         *
         * The message "Out of range." is forwarded to the base Exception class.
         */
        OutOfRange()
            : Exception("Out of range.")
        {}
    };
}