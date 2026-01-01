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
#include "Celery/Algo/Itoa.h"

namespace Celery::Algorithm
{
    /**
     * @brief Convert a floating-point value to text by writing characters to an adapter.
     *
     * This function formats a floating-point number by:
     *  - Handling special values: NaN and Infinity.
     *  - Writing an optional leading '-' sign for negative values.
     *  - Writing the integer portion using the existing Itoa routine.
     *  - Writing a decimal point followed by up to @p precision fractional digits.
     *
     * The Adapter type must provide a method with signature:
     * @code
     * void Write(char c);
     * @endcode
     *
     * This routine does not perform locale-aware formatting and does not
     * perform decimal rounding of the last digit — it truncates fractional digits
     * produced by successive multiplication by 10. The conversion stops early
     * when the remaining fractional part is less than or equal to
     * std::numeric_limits<FloatType>::epsilon().
     *
     * @tparam Adapter  Type that receives character output; must implement Write(char).
     * @tparam Value    Floating-point type (or reference/const-qualified variant).
     *
     * @param value      The floating-point value to convert.
     * @param adapter    Output adapter (forwarding reference). Characters are emitted
     *                   via adapter.Write(char).
     * @param precision  Maximum number of digits to emit after the decimal point.
     *                   Defaults to 6.
     *
     * @note For very large integer parts that exceed uint64_t when cast, the
     *       integer portion will be truncated by the static_cast to uint64_t.
     * @note The function is intended for simple, efficient conversions where
     *       exact decimal rounding and edge-case IEEE formatting are not required.
     */
    template<
        typename Adapter,
        typename Value,
        typename = decltype(std::declval<Adapter>().Write(char{})),
        typename = std::enable_if_t<
            std::is_floating_point_v<
                std::remove_cv_t<std::remove_reference_t<Value>>
            >
        >
    >
    void Ftoa(Value value, Adapter &&adapter, int precision = 6)
    {
        using FloatType = std::remove_cv_t<std::remove_reference_t<Value>>;

        // Handle NaN and Inf
        if (std::isnan(value))
        {
            adapter.Write('n'); adapter.Write('a'); adapter.Write('n');
            return;
        }

        if (std::isinf(value))
        {
            if (value < 0) adapter.Write('-');
            adapter.Write('i'); adapter.Write('n'); adapter.Write('f');
            return;
        }

        // Handle sign
        if (value < 0)
        {
            adapter.Write('-');
            value = -value;
        }

        // Extract integer part
        auto int_part = static_cast<uint64_t>(value);
        auto frac_part = value - static_cast<FloatType>(int_part);

        // Write integer part using existing Itoa logic
        Itoa(int_part, adapter);

        adapter.Write('.');

        // Write fractional digits
        for (int i = 0; i < precision; ++i)
        {
            frac_part *= 10;
            int digit = static_cast<int>(frac_part);
            adapter.Write(static_cast<char>('0' + digit));
            frac_part -= digit;
            if (frac_part <= std::numeric_limits<FloatType>::epsilon())
                break;
        }
    }
}
