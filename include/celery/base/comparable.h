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
#include <concepts>

namespace Celery::Base
{
    /**
     * @brief Concept to check if a type T supports default equality comparison.
     *
     * This concept requires that the type T has valid `operator==` and
     * `operator!=` that return a type convertible to bool.
     *
     * @tparam T The type to check for equality comparability.
     */
    template <typename T>
    concept DefaultEqualityComparable = requires(T a, T b)
    {
        { a == b } -> std::convertible_to<bool>;
        { a != b } -> std::convertible_to<bool>;
    };

    /**
     * @brief Concept to check if a type T supports default arithmetic comparison.
     *
     * This concept requires that the type T has valid `operator<`, `operator>`,
     * `operator<=`, and `operator>=` that return a type convertible to bool.
     *
     * @tparam T The type to check for arithmetic comparability.
     */
    template <typename T>
    concept DefaultArithmeticComparable = requires(T a, T b)
    {
        { a < b } -> std::convertible_to<bool>;
        { a > b } -> std::convertible_to<bool>;
        { a <= b } -> std::convertible_to<bool>;
        { a >= b } -> std::convertible_to<bool>;
    };

    /**
     * @brief CRTP mixin that provides equality comparison operations.
     *
     * @note This class provides static methods Eq and Neq for equality
     *       and inequality comparisons. By default, these methods are
     *       deleted and must be specialized for specific types.
     */
    template <typename T>
    struct EqualityCompare
    {
        /**
         * @brief Compare two objects of type T for equality using default operators.
         *
         * @param a First object to compare.
         * @param b Second object to compare.
         * @return true if a and b are equal, false otherwise.
         */
        static bool Eq(const T& a, const T& b)
            requires DefaultEqualityComparable<T>
        {
            return a == b;
        }

        /**
         * @brief Compare two objects of type T for inequality using default operators.
         *
         * @param a First object to compare.
         * @param b Second object to compare.
         * @return true if a and b are not equal, false otherwise.
         */
        static bool Neq(const T& a, const T& b)
            requires DefaultEqualityComparable<T>
        {
            return a != b;
        }

        /**
         * @brief Compare two objects of type T for equality.
         *
         * @return true if a and b are equal, false otherwise.
         *
         * @note This function is deleted by default. Specializations
         *       must provide an implementation.
         */
        static bool Eq(const T&, const T&) = delete;

        /**
         * @brief Compare two objects of type T for inequality.
         *
         * @return true if a and b are not equal, false otherwise.
         *
         * @note This function is deleted by default. Specializations
         *       must provide an implementation.
         */
        static bool Neq(const T&, const T&) = delete;
    };

    /**
     * @brief CRTP mixin that provides arithmetic comparison operations.
     *
     * @note This class provides static methods Lt, Gt, Lte, and Gte for
     *       less-than, greater-than, less-than-or-equal, and
     *       greater-than-or-equal comparisons. By default, these methods
     *       are deleted and must be specialized for specific types.
     */
    template <typename T>
    struct ArithmeticCompare
    {
        /**
         * @brief Compare two objects of type T for less-than using default operators.
         *
         * @param a First object to compare.
         * @param b Second object to compare.
         * @return true if a is less than b, false otherwise.
         */
        static bool Lt(const T& a, const T& b)
            requires DefaultArithmeticComparable<T>
        {
            return a < b;
        }

        /**
         * @brief Compare two objects of type T for greater-than using default operators.
         *
         * @param a First object to compare.
         * @param b Second object to compare.
         * @return true if a is greater than b, false otherwise.
         */
        static bool Gt(const T& a, const T& b)
            requires DefaultArithmeticComparable<T>
        {
            return a > b;
        }

        /**
         * @brief Compare two objects of type T for less-than-or-equal using default operators.
         *
         * @param a First object to compare.
         * @param b Second object to compare.
         * @return true if a is less than or equal to b, false otherwise.
         */
        static bool Lte(const T& a, const T& b)
            requires DefaultArithmeticComparable<T>
        {
            return a <= b;
        }

        static bool Gte(const T& a, const T& b)
            requires DefaultArithmeticComparable<T>
        {
            return a >= b;
        }

        /**
         * @brief Compare two objects of type T for less-than.
         *
         * @note This function is deleted by default. Specializations
         *       must provide an implementation.
         */
        static bool Lt(const T&, const T&) = delete;

        /**
         * @brief Compare two objects of type T for greater-than.
         *
         * @note This function is deleted by default. Specializations
         *       must provide an implementation.
         */
        static bool Gt(const T&, const T&) = delete;

        /**
         * @brief Compare two objects of type T for less-than-or-equal.
         *
         * @note This function is deleted by default. Specializations
         *       must provide an implementation.
         */
        static bool Lte(const T&, const T&) = delete;

        /**
         * @brief Compare two objects of type T for greater-than-or-equal.
         *
         * @note This function is deleted by default. Specializations
         *       must provide an implementation.
         */
        static bool Gte(const T&, const T&) = delete;
    };

    /**
     * @brief Concept that checks if a type T supports equality comparison.
     *
     * A type T is considered EqualityComparable if it provides
     * static methods Eq and Neq in the EqualityCompare struct
     * that return bool when called with two T objects.
     */
    template <typename T>
    concept EqualityComparable = requires(T &&a, T &&b)
    {
        {
            EqualityCompare<T>::Eq(std::forward<T>(a), std::forward<T>(b))
        } -> std::convertible_to<bool>;

        {
            EqualityCompare<T>::Neq(std::forward<T>(a), std::forward<T>(b))
        } -> std::convertible_to<bool>;
    };

    /**
     * @brief Concept that checks if a type T supports arithmetic comparison.
     *
     * A type T is considered ArithmeticComparable if it provides
     * static methods Lt, Gt, Lte, and Gte in the ArithmeticCompare struct
     * that return bool when called with two T objects.
     */
    template <typename T>
    concept ArithmeticComparable = requires(T &&a, T &&b)
    {
        {
            ArithmeticCompare<T>::Lt(std::forward<T>(a), std::forward<T>(b))
        } -> std::convertible_to<bool>;

        {
            ArithmeticCompare<T>::Gt(std::forward<T>(a), std::forward<T>(b))
        } -> std::convertible_to<bool>;

        {
            ArithmeticCompare<T>::Lte(std::forward<T>(a), std::forward<T>(b))
        } -> std::convertible_to<bool>;

        {
            ArithmeticCompare<T>::Gte(std::forward<T>(a), std::forward<T>(b))
        } -> std::convertible_to<bool>;
    };

    /**
     * @brief Concept that checks if a type T supports both equality and arithmetic comparison.
     *
     * A type T is considered Comparable if it satisfies both
     * the EqualityComparable and ArithmeticComparable concepts.
     */
    template <typename T>
    concept Comparable = EqualityComparable<T> && ArithmeticComparable<T>;

    /**
     * @brief Concept that checks if a type T has a specialization of EqualityCompare.
     *
     * A type T is considered to have an EqualityCompare specialization
     * if the static method Eq is defined and returns bool when called
     * with two T objects.
     */
    template <typename T>
    concept HasEqualityEspecialization =
        requires(T a, T b) {
            { EqualityCompare<T>::Eq(a, b) } ->
                std::convertible_to<bool>;
        };

    /**
     * @brief Concept that checks if a type T has a specialization of ArithmeticCompare.
     *
     * A type T is considered to have an ArithmeticCompare specialization
     * if the static method Lt is defined and returns bool when called
     * with two T objects.
     */
    template <typename T>
    concept HasArithmeticEspecialization =
        requires(T a, T b) {
            { ArithmeticCompare<T>::Lt(a, b) } ->
                std::convertible_to<bool>;
        };

    /**
     * @brief Concept that checks if a type T has specializations of both EqualityCompare and ArithmeticCompare.
     *
     * A type T is considered to have both specializations if it satisfies
     * the HasEqualityEspecialization and HasArithmeticEspecialization concepts.
     */    template <typename T>
    concept HasComparableEspecialization =
        HasEqualityEspecialization<T> && HasArithmeticEspecialization<T>;
}