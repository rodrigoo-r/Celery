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
#include "Celery/Except/OutOfRange.h"
#include "Celery/Trait/Type.h"

namespace Celery::Misc
{
    /**
     * @brief A simple optional container that may or may not hold a value of type T.
     *
     * This class provides storage for an object of type T using
     * placement new and tracks whether a value is currently
     * engaged (present) or not. It supports construction,
     * assignment, and access to the contained value.
     *
     * @tparam T The type of the optional value.
     */
    template <typename T>
    class Optional
    {
    protected:
        alignas (T) unsigned char storage[sizeof(T)];
        bool engaged = false;

        /**
         * @brief Helper method to construct from another Optional.
         *
         * If the other Optional is engaged, copies its value into this one.
         *
         * @param other The other Optional to copy from.
         */
        template <bool IsConstruct = false>
        void ConstructFrom(const Optional &other)
        {
            // Avoid self-assignment during assignment operations
            if constexpr (!IsConstruct)
            {
                if (this == &other) return; // Self-assignment check
            }

            if (engaged)
            {
                reinterpret_cast<T *>(storage)->~T();
                engaged = false;
            }

            if (other.engaged)
            {
                new (storage) T(other.Value());
                engaged = true;
            }
        }

        /**
         * @brief Helper method to move-construct from another Optional.
         *
         * If the other Optional is engaged, moves its value into this one
         * and disengages the other.
         *
         * @param other The other Optional to move from.
         */
        void ConstructFrom(Optional &&other) noexcept
        {
            if (engaged)
            {
                reinterpret_cast<T *>(storage)->~T();
                engaged = false;
            }

            if (other.engaged)
            {
                new (storage) T(std::move(other.Value()));
                engaged = true;

                reinterpret_cast<T *>(other.storage)->~T();
                other.engaged = false;
            }
        }

    public:
        /**
         * @brief Default constructor creates an empty Optional.
         */
        Optional() = default;

        /**
         * @brief Constructs an Optional with the given value.
         *
         * Uses placement new to construct the value in the aligned storage.
         *
         * @tparam U Type of the value to store (must be the same as T).
         * @param value The value to store in the Optional.
         */
        template <
            typename U = T,
            typename = Trait::EnsureSame<U, T>
        >
        Optional(U &&value)
        {
            new (storage) T(std::forward<U>(value));
            engaged = true;
        }

        /**
         * @brief Copy constructor.
         *
         * If the other Optional is engaged, copies its value into this one.
         *
         * @param other The other Optional to copy from.
         */
        Optional(const Optional &other)
        {
            ConstructFrom<true>(other);
        }

        /**
         * @brief Move constructor.
         *
         * If the other Optional is engaged, moves its value into this one
         * and disengages the other.
         *
         * @param other The other Optional to move from.
         */
        Optional(Optional &&other) noexcept
        {
            ConstructFrom(other);
        }

        /**
         * @brief Copy assignment operator.
         *
         * If the other Optional is engaged, copies its value into this one,
         * destroying any existing value.
         *
         * @param other The other Optional to copy from.
         * @return Reference to this Optional after assignment.
         */
        Optional &operator=(const Optional &other)
        noexcept {
            ConstructFrom(other);
            return *this;
        }

        /**
         * @brief Move assignment operator.
         *
         * If the other Optional is engaged, moves its value into this one,
         * destroying any existing value and disengaging the other.
         *
         * @param other The other Optional to move from.
         * @return Reference to this Optional after assignment.
         */
        Optional &operator=(Optional &&other) noexcept
        {
            ConstructFrom(std::forward<decltype(other)>(other));
            return *this;
        }

        /**
         * @brief Destructor.
         *
         * If engaged, calls the destructor of the contained value.
         */
        ~Optional()
        {
            if (engaged)
            {
                reinterpret_cast<T *>(storage)->~T();
            }
        }

        /**
         * @brief Assignment operator.
         *
         * If this Optional is already engaged, destroys the current value
         * before constructing the new value in place.
         *
         * @tparam U Type of the value to assign (must be the same as T).
         * @param other The value to assign to this Optional.
         * @return Reference to this Optional after assignment.
         */
        template <
            typename U = T,
            typename = Trait::EnsureSame<U, T>
        >
        Optional &operator=(U &&other)
        {
            if (engaged)
            {
                reinterpret_cast<T *>(storage)->~T();
            }

            new (storage) T(std::forward<U>(other));
            engaged = true;
            return *this;
        }

        /**
         * @brief Checks if the Optional currently holds a value.
         *
         * @return true if a value is present, false otherwise.
         */
        [[nodiscard]] bool HasValue() const
        {
            return engaged;
        }

        /**
         * @brief Accessor for the contained value.
         *
         * Throws Except::OutOfRange if no value is present.
         *
         * @return Reference to the contained value.
         * @throws Except::OutOfRange if no value is present.
         */
        T &Value()
        {
            if (!engaged)
            {
                throw Except::OutOfRange();
            }

            return *reinterpret_cast<T *>(storage);
        }

		/**
         * @brief Const accessor for the contained value.
         *
         * Throws Except::OutOfRange if no value is present.
         *
         * @return Const reference to the contained value.
         * @throws Except::OutOfRange if no value is present.
         */
		const T& Value() const
		{
    		return *reinterpret_cast<const T*>(storage);
		}

        /**
         * @brief Member access operator for the contained value.
         *
         * Throws Except::OutOfRange if no value is present.
         *
         * @return Pointer to the contained value.
         * @throws Except::OutOfRange if no value is present.
         */
        T *operator->()
        {
            if (!engaged)
            {
                throw Except::OutOfRange();
            }

            return reinterpret_cast<T *>(storage);
        }

        /// Creates an Optional containing a value
        static Optional Some(const T& value)
        {
            return Optional(value);
        }

        /// Creates an Optional containing a moved value
        static Optional Some(T&& value)
        {
            return Optional(std::move(value));
        }

        /// Creates an empty Optional
        static Optional None()
        {
            return Optional();
        }

		template <typename... Args>
		static Optional Emplace(Args&&... args)
		{
    		Optional opt;
    		new (opt.storage) T(std::forward<Args>(args)...);
    		opt.engaged = true;
    		return opt;
		}
    };
} // namespace Celery::Misc