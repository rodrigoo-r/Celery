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
    /**
     * @brief Convert an integral value to decimal text by writing characters into an Adapter.
     *
     * This function formats an integral `Value` into its base-10 textual representation
     * and emits each character via `adapter.Write(char)`. It supports signed and
     * unsigned integral types. For signed negative values a leading '-'
     * is emitted. Zero is handled explicitly.
     *
     * @tparam Adapter Type providing a callable method `Write(char)`.
     *         The adapter may be an lvalue or rvalue and is forwarded accordingly.
     * @tparam Value Integral value type to convert. Must be an integral type
     *         (cv/ref qualifiers are removed by the SFINAE above).
     * @pre `Adapter` must have a member callable `Write(char)` visible in the
     *      context where this template is instantiated.
     * @pre `Value` must be an integral type.
     * @param value The integral value to convert to decimal text.
     * @param adapter Adapter instance that will receive output characters via `Write`.
     * @note If `value == 0`, the function writes a single `'0'` and returns.
     * @note For signed types, a leading `'-'` is emitted for negative values.
     * @warning Negating the most-negative signed integer (for example `INT_MIN`)
     *          may overflow and result in implementation-defined behavior; callers
     *          should avoid relying on wrap-around semantics for that edge case.
     * @example
     * struct BufferAdapter { void Write(char c) { buffer.push_back(c); } std::string buffer; };
     * BufferAdapter buf; Itoa(-123, buf); // buf.buffer == "-123"
     */
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
        using ValueType = std::remove_cv_t<
            std::remove_reference_t<Value>
        >;
        using UValue = std::make_unsigned_t<ValueType>;
        using AbsValueType = std::conditional_t<
            std::is_unsigned_v<ValueType>,
            ValueType,
            UValue
        >;

        // Handle zero explicitly
        if (value == 0)
        {
            adapter.Write('0');
            return;
        }

        // Handle negative values for signed types
        bool negative = false;
        if constexpr (!std::is_unsigned_v<ValueType>)
        {
            // Check only if the value type is signed
            if (value < 0)
            {
                negative = true; // Mark as negative
                adapter.Write('-');
            }
        }

        // Work with absolute value for digit extraction
        AbsValueType &abs_value = value;

        // Change only for signed types
        if constexpr (std::is_signed_v<ValueType>)
        {
            if (negative)
            {
                // Handle potential overflow when negating
                abs_value = static_cast<UValue>(-(value + 1)) + 1;
            }
        }

        // Find the highest power of 10 less than or equal to value
        AbsValueType div = 1;
        while (abs_value / div >= 10)
            div *= 10;

        // Write digits from most to least significant
        while (div > 0)
        {
            const int digit = static_cast<int>(abs_value / div);
            adapter.Write(static_cast<char>('0' + digit));
            abs_value %= div;
            div /= 10;
        }
    }
}