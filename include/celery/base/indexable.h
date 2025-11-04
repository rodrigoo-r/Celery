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
#include "bufferable.h"
#include "celery/except/out_of_range.h"
#include "celery/trait/default.h"
#include "celery/util/reindex.h"
#include "iterable.h"
#include "sizeable.h"

namespace Celery::Base
{
    /**
     * @brief A minimal indexable container base class.
     *
     * `Indexable` provides array-like element access via the subscript
     * operator (`operator[]`) and integrates with `Sizeable` for length
     * tracking. It stores a raw pointer to contiguous elements of type `T`.
     *
     * This class does not own the memory by itself; subclasses or users
     * are responsible for allocation, deallocation and ensuring `data`
     * and `len` are consistent.
     *
     * @tparam T Element type stored by the container.
     */
    template<typename T>
    class Indexable : public Sizeable, public Bufferable<T>
    {
    public:
        /**
         * @brief Non-const subscript operator with bounds checking.
         *
         * Provides mutable access to the element at the given `index`.
         * Throws `Except::OutOfRange` if `index` is greater than or equal
         * to the current length (`len`) inherited from `Sizeable`.
         *
         * @param index Position of the element to access.
         * @return Reference to the element at `index`.
         * @throws Except::OutOfRange When `index >= len`.
         */
        T &operator[](Trait::VeryLarge index)
        {
            if (index >= this->len)
            {
                throw Except::OutOfRange();
            }

            return this->data[index];
        }

        /**
         * @brief Const subscript operator with bounds checking.
         *
         * Provides read-only access to the element at the given `index`.
         * Throws `Except::OutOfRange` if `index` is out of bounds.
         *
         * @param index Position of the element to access.
         * @return Const reference to the element at `index`.
         * @throws Except::OutOfRange When `index >= len`.
         */
        const T &operator[](size_t index) const
        {
            if (index >= this->len)
            {
                throw Except::OutOfRange();
            }

            return this->data[index];
        }

        /**
         * @brief Get a pointer to the first element.
         *
         * @return Pointer to the first element in the container.
         */
        T Front()
        {
            return operator[](0);
        }

        /**
         * @brief Get a pointer to the last element.
         *
         * @return Pointer to the last element in the container.
         */
        T *Back()
        {
            return operator[](this->len - 1);
        }

        /**
         * @brief Get an iterable for range-based for loops.
         *
         * @return Iterable starting at the beginning of the container.
         */
        Iterable<T> begin()
        {
            return Iterable<T>(this->data);
        }

        /**
         * @brief Get an iterable representing the end of the container.
         *
         * @return Iterable pointing one past the last element.
         */
        Iterable<T> end()
        {
            return Iterable<T>(this->data + this->len);
        }

        /**
         * @brief Append an element to the container using += operator.
         *
         * Pure virtual; must be implemented by subclasses to define
         * how elements are added.
         *
         * @param value Element to append.
         * @return Reference to this container after appending.
         */
        virtual Indexable &operator+=(const T &value) = 0;

        /**
         * @brief Append an element to the container using += operator.
         *
         * Pure virtual; must be implemented by subclasses to define
         * how elements are added.
         *
         * @param value Element to append.
         * @return Reference to this container after appending.
         */
        virtual Indexable &operator+=(T &value) = 0;

        /**
         * @brief Append an element to the container using += operator.
         *
         * Pure virtual; must be implemented by subclasses to define
         * how elements are added.
         *
         * @param value Element to append.
         * @return Reference to this container after appending.
         */
        virtual Indexable &operator+=(T &&value) = 0;

        /**
         * @brief Remove an element at the specified index using -= operator.
         *
         * Reindexes the internal data to remove the element at `idx`.
         * Throws `Except::OutOfRange` if `idx` is out of bounds.
         *
         * @param idx Index of the element to remove.
         * @return Reference to this container after removal.
         * @throws Except::OutOfRange When `idx >= len`.
         */
        Indexable &operator-=(const Trait::VeryLarge &idx)
        {
            Utility::Reindex(this->data, this->len, idx);
            return *this;
        }

        /**
         * @brief Remove an element at the specified index using -= operator (const).
         *
         * Reindexes the internal data to remove the element at `idx`.
         * Throws `Except::OutOfRange` if `idx` is out of bounds.
         *
         * @param idx Index of the element to remove.
         * @return Reference to this container after removal.
         * @throws Except::OutOfRange When `idx >= len`.
         */
        Indexable &operator-=(Trait::VeryLarge &idx)
        {
            Utility::Reindex(this->data, this->len, idx);
            return *this;
        }

        /**
         * @brief Remove an element at the specified index using -= operator (rvalue).
         *
         * Reindexes the internal data to remove the element at `idx`.
         * Throws `Except::OutOfRange` if `idx` is out of bounds.
         *
         * @param idx Index of the element to remove.
         * @return Reference to this container after removal.
         * @throws Except::OutOfRange When `idx >= len`.
         */
        Indexable &operator-=(Trait::VeryLarge &&idx)
        {
            Utility::Reindex(this->data, this->len, idx);
            return *this;
        }

        /**
         * @brief Virtual default destructor.
         *
         * Declared `override` to ensure proper polymorphic destruction through
         * `Sizeable` pointers. No cleanup is performed by this base class.
         */
        ~Indexable() override = default;
    };
}