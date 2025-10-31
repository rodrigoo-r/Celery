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
#include "celery/string/string.h"

namespace Celery::Base
{
    /**
     * @brief Base class providing a ToString method for derived types.
     *
     * This class defines a virtual ToString method that derived classes
     * can override to provide a string representation of the object.
     * The default implementation returns a placeholder string.
     */
    class Display
    {
    public:
        /**
         * @brief Virtual destructor.
         *
         * Ensures proper cleanup of derived classes through base-class pointers.
         */
        virtual ~Display() = default;

        /**
         * @brief Convert the object to a string representation.
         *
         * Derived classes should override this method to return a meaningful
         * string representation of the object.
         *
         * @return A String representing the object.
         */
        [[nodiscard]] virtual Str::String ToString()
        const noexcept = 0;
    };
}