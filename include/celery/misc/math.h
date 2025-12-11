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

namespace Celery::Misc 
{
#   if defined(_WIN64) || defined(__x86_64__) || defined(__ppc64__)
    using MagicType = __uint128_t;
#   else
    using MagicType = uint32_t;
#   endif

    template <
        typename T,
        typename U,
        typename Return = T,
        typename = std::enable_if_t<std::is_integral_v<T>>,
        typename = std::enable_if_t<std::is_integral_v<U>>,
        typename = std::enable_if_t<std::is_integral_v<Return>>
    >
    [[nodiscard]] constexpr Return Max(T a, U b)
    {
        if (a < b)
        {
            if constexpr (std::is_same_v<Return, T>)
            {
                return b;
            }
            else
            {
                return static_cast<Return>(b);
            }
        }

        if constexpr (std::is_same_v<Return, T>)
        {
            return a;
        }
        else
        {
            return static_cast<Return>(a);
        }
    }

    template <
        typename T,
        typename U,
        typename Return = T,
        typename = std::enable_if_t<std::is_integral_v<T>>,
        typename = std::enable_if_t<std::is_integral_v<U>>,
        typename = std::enable_if_t<std::is_integral_v<Return>>
    >
    [[nodiscard]] constexpr Return Min(T a, U b)
    {
        if (a > b)
        {
            if constexpr (std::is_same_v<Return, T>)
            {
                return b;
            }
            else
            {
                return static_cast<Return>(b);
            }
        }

        if constexpr (std::is_same_v<Return, T>)
        {
            return a;
        }
        else
        {
            return static_cast<Return>(a);
        }
    }

    /**
     * \brief Computes the magic number used for fast modulo reduction.
     *
     * This function calculates a precomputed "magic" value for a given maximum divisor,
     * which can be used to replace division with faster multiplication and bit-shifting.
     * The implementation uses 128-bit arithmetic on 64-bit platforms and 32/64-bit on others.
     *
     * \param max The divisor for which to compute the magic number.
     * \return The computed magic number for use in fast modulo operations.
     */
    [[nodiscard]] inline MagicType MagicNumber(const size_t max)
    {
#   if defined(_WIN64) || defined(__x86_64__) || defined(__ppc64__)
        return (static_cast<MagicType>(1) << 64) / max;
#   else
        return (MagicType(1) << 32) / max;
#   endif
    }

    /**
     * \brief Computes the modulo of two numbers using a precomputed magic number.
     *
     * This function performs a fast modulo operation using a precomputed magic number,
     * replacing division with multiplication and bit-shifting for efficiency.
     * The implementation uses 128-bit arithmetic on 64-bit platforms and 32/64-bit on others.
     *
     * \param a The dividend.
     * \param b The divisor.
     * \param magic The precomputed magic number for the divisor.
     * \return The result of a % b.
     */
    inline size_t Mod(const size_t a, const size_t b, const MagicType magic)
    {
#if defined(_WIN64) || defined(__x86_64__) || defined(__ppc64__)
        const __uint128_t prod = static_cast<__uint128_t>(a) * magic;
        const size_t quotient = prod >> 64;
        return a - quotient * b;
#else
        const uint64_t prod = static_cast<uint64_t>(a) * magic;
        const size_t quotient = prod >> 32;
        return a - quotient * b;
#endif
    }
}