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

namespace Celery::Algorithm
{
    template<
        typename Adapter,
        typename Value,
        // SFINAE to make sure .Write(char) exists
        typename = decltype(
            std::declval<Adapter>().Write(char{})
        ),
        typename = std::enable_if_t<
            std::is_integral_v<
                std::remove_cv_t<
                    std::remove_reference_t<Value>
                >
            >
        >
    >
    void Itoa(
        Value value,
        Adapter &&adapter
    )
    {
        // Handle zero explicitly
        if (value == 0)
        {
            adapter.Write('0');
            return;
        }

        // Handle negative values for signed types
        const bool negative = value < 0;
        if (negative)
        {
            adapter.Write('-');
            value = -value;
        }

        // Find the highest power of 10 less than or equal to value
        Value div = 1;
        while (value / div >= 10)
            div *= 10;

        // Write digits from most to least significant
        while (div > 0)
        {
            int digit = static_cast<int>(value / div);
            adapter.Write(static_cast<char>('0' + digit));
            value %= div;
            div /= 10;
        }
    }
}