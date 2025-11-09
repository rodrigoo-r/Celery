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
#include "celery/trait/default.h"

namespace Celery::Base
{
    /**
     * @brief Abstract interface for resizable objects within the Celery::Base namespace.
     *
     * Provides a minimal interface to represent objects that maintain a capacity
     * and can change that capacity via the virtual Resize method.
     */

    /**
     * @brief Abstract base class representing a resizable entity.
     *
     * Derive from this class to implement objects that expose a capacity and
     * support resizing that capacity. The concrete class is responsible for
     * enforcing any invariants or bounds on capacity and performing any
     * necessary allocation or reconfiguration when Resize is called.
     */
    class Resizable
    {
    protected:
        /**
         * @brief The current capacity of the object.
         *
         * Type is provided by `Trait::VeryLarge` (see `celery/trait/default.h`).
         * Initialized to 0. Concrete implementations may read or update this
         * value as part of their Resize implementation or other internal logic.
         */
        Trait::VeryLarge capacity = 0;

    public:
        /**
         * @brief Virtual default destructor.
         *
         * Ensures proper cleanup of derived classes through base-class pointers.
         */
        virtual ~Resizable() = default;

        /**
         * @brief Get the current capacity of the object.
         *
         * @return Trait::VeryLarge The current capacity.
         */
        [[nodiscard]] Trait::VeryLarge Capacity() const
        {
            return capacity;
        }

        /**
         * @brief Resize the object to a new capacity.
         *
         * Implementations must define how the object adjusts internal storage or
         * state to accommodate `new_capacity`. Behavior for shrinking, growing,
         * and invalid values should be documented and enforced by each derived class.
         *
         * @param new_capacity The desired capacity after resizing.
         */
        virtual void Resize(Trait::VeryLarge new_capacity) = 0;
    };
}
