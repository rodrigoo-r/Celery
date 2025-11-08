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

#include "celery/io/stream.h"
#include "celery/string/string.h"

namespace Celery::Serialize
{
    /**
     * @brief CRTP mixin that provides serialization and raw display capabilities.
     *
     * By default, the methods are deleted and must be specialized for specific types.
     * Specializations should implement:
     *  - static Str::String ToString(const T &): Converts the object to a string representation.
     *  - static void Raw(const T &, Io::Pmr::OStream<> &): Outputs a raw representation to the stream.
     *
     * @tparam T The type to provide serialization/display for.
     */
    template<typename T>
    struct Display
    {
        /*
         *  @brief Convert the object to a string representation.
         *
         * @param obj The object to convert.
         * @return Str::String The string representation of the object.
         *
         * @note This method must be specialized for each type T.
         */
        static inline Str::String ToString(T &&) = delete;

        /*
         *  @brief Output a raw representation of the object to the provided stream.
         *
         * @param obj The object to output.
         * @param stream The output stream to write to.
         *
         * @note This method must be specialized for each type T.
         */
        static inline void Raw(T &&, Io::Pmr::OStream<> &) = delete;
    };

    /**
     * @brief Concept that checks if a type T supports partial display (ToString).
     *
     * A type T is considered PartlyDisplayable if it provides
     * a static method ToString in the Display struct
     * that returns a Str::String when called with a const T&.
     */
    template<typename T>
    concept PartlyDisplayable = requires(T &&value)
    {
        { Display<T>::ToString(std::forward<T>(value)) }
            -> std::convertible_to<Str::String>;
    };

    /**
     * @brief Concept that checks if a type T supports raw display (Raw).
     *
     * A type T is considered RawDisplayable if it provides
     * a static method Raw in the Display struct
     * that can be called with a const T& and an Io::Pmr::OStream<> &.
     */
    template<typename T>
    concept RawDisplayable = requires(T &&value)
    {
        { Display<T>::Raw(std::forward<T>(value)) };
    };

    /**
     * @brief Concept that checks if a type T supports both partial and raw display.
     *
     * A type T is considered FullyDisplayable if it satisfies
     * both the PartlyDisplayable and RawDisplayable concepts.
     */
    template<typename T>
    concept FullyDisplayable = PartlyDisplayable<T> && RawDisplayable<T>;
}