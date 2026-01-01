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
     * @class Unimplemented
     * @brief Exception thrown when a requested feature or function is not implemented.
     *
     * Use this exception to mark code paths that are intentionally left
     * unimplemented (for example, as stubs during development).
     */
    class Unimplemented : public Exception
    {
    public:
        /**
         * @brief Construct a new UnimplementedException with a default message.
         *
         * The default diagnostic message is "Functionality not yet implemented."
         */
        Unimplemented()
            : Exception("Functionality not yet implemented.")
        {}
    };
}