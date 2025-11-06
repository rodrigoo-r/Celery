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

#include "celery/algo/ftoa.h"
#include "celery/base/display.h"
#include "celery/string/external.h"
#include "stream.h"

namespace Celery::Io
{
    /**
     * @brief Write a single value to the library output (stdout/stream).
     *
     * This template performs compile-time type-dispatch using `if constexpr`
     * and standard type traits to choose the appropriate serialization and
     * write method for the provided argument.
     *
     * Supported cases (checked in order):
     * - Types derived from `Celery::Base::Display`: calls `ToString()` and writes the resulting `Str::String`.
     * - `char`: written as a single character via `IStdout.Write`.
     * - Types convertible to `const char *`: written as a C-string via `IStdout.Batch`.
     * - Types convertible to `Str::String` or `Str::External`: converted and written via `IStdout.Batch`.
     * - `bool`: written as the textual `"true"` or `"false"`.
     * - Integral types: serialized using `Algorithm::Itoa`.
     * - Floating-point types: serialized using `Algorithm::Ftoa`.
     *
     * If none of the above cases match, a `static_assert(false, ...)` is emitted
     * to require explicit support (e.g., by deriving from `Base::Display` and
     * implementing `ToString()`).
     *
     * @tparam T Type of the argument to write. CV and reference qualifiers are stripped internally.
     * @param arg The value to write to the output.
     *
     * @note This function forwards to either `Stdout::Write`, `IStdout.Write`, `IStdout.Batch`,
     *       `Algorithm::Itoa`, or `Algorithm::Ftoa` depending on the detected type.
     */
    template <typename T>
    void WriteSingle(T &&arg)
    {
        // Convert to string if possible
        if constexpr (
            std::is_base_of_v<
                Base::Display,
                std::remove_cv_t<std::remove_reference_t<T>>
            >
        )
        {
            // Use ToString method
            Str::String str = arg.ToString();
            IStdout.Batch(str.Ptr(), str.Len());
        }
        else if constexpr  (std::is_same_v<std::remove_cv_t<std::remove_reference_t<T>>, char>)
        {
            // Single character
            const char c = static_cast<char>(arg);
            IStdout.Write(c);
        }
        else if constexpr (std::is_convertible_v<T, const char *>)
        {
            // Direct C-string
            const char *cstr = static_cast<const char *>(arg);
            const auto len = strlen(cstr);
            IStdout.Batch(cstr, len);
        }
        else if constexpr (std::is_convertible_v<T, Str::String> || std::is_convertible_v<T, Str::External>)
        {
            // Convert to Celery String
            auto str = static_cast<
                std::conditional_t<
                    std::is_convertible_v<T, Str::String>,
                    Str::String,
                    Str::External
                >
            >(arg);
            IStdout.Batch(str.Ptr(), str.Len());
        }
        // Handle primitive types
        else if (std::is_same_v<std::remove_cv_t<std::remove_reference_t<T>>, bool>)
        {
            // Boolean type
            const char *bool_str = arg ? "true" : "false";
            IStdout.Batch(bool_str, arg ? 4 : 5); // Length of "true" or "false"
        }
        else if constexpr (
            std::is_integral_v<std::remove_cv_t<std::remove_reference_t<T>>>
        )
        {
            // Integral type
            Algorithm::Itoa(arg, IStdout);
        }
        else if constexpr (std::is_floating_point_v<std::remove_cv_t<std::remove_reference_t<T>>>)
        {
            Algorithm::Ftoa(arg, IStdout);
        }
        else
        {
            static_assert(
                false,
                "Unsupported type for Print/Println. To "
                "enable printing for a custom type, "
                "derive from Celery::Base::Display and "
                "implement ToString()."
            );
        }
    }

    /**
     * @brief Print one or more values to the output without a trailing newline.
     *
     * Expands the parameter pack and calls `WriteSingle` for each argument in
     * left-to-right order. Perfect-forwards each argument to preserve value
     * category and qualifiers.
     *
     * @tparam Args Parameter pack of argument types.
     * @param args Arguments to print.
     */
    template <typename... Args>
    void Print(Args &&... args)
    {
        // Expand and write each argument
        (WriteSingle(std::forward<Args>(args)), ...);
    }

    /**
     * @brief Print one or more values followed by a newline.
     *
     * This calls `Print` for the provided arguments and then writes a single
     * newline character by calling `Print('\n')`.
     *
     * @tparam Args Parameter pack of argument types.
     * @param args Arguments to print before the newline.
     */
    template <typename... Args>
    void Println(Args &&... args)
    {
        Print(std::forward<Args>(args)...);
        Print('\n');
    }
}
