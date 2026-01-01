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
#include <algorithm>
#include <type_traits>


#include "celery/except/out_of_range.h"
#include "celery/trait/default.h"

namespace Celery::Utility
{
    /**
     * @brief Remove an element from a raw array by shifting elements left.
     *
     * This function shifts elements in-place to overwrite the element at
     * `to_remove`. It chooses the most efficient strategy available for the
     * element type `T`:
     * - If `T` is trivially copyable, a direct assignment is used.
     * - Else if `T` is move-assignable, `std::move` assignment is used.
     * - Otherwise a placement-new with move-construction is used and the
     *   moved-from object's destructor is invoked.
     *
     * @tparam T Element type stored in the array.
     * @param[in,out] data Reference to a pointer to the array buffer. The buffer
     *                    contents are modified but the pointer value is not
     *                    reallocated by this function.
     * @param[in] len Number of valid elements currently stored in the buffer.
     * @param[in] to_remove Index of the element to remove (zero-based).
     *
     * @pre `data` points to an array of at least `len` elements.
     * @pre `0 <= to_remove < len`.
     * @post Elements at indices `to_remove+1 .. len-1` are shifted left by one
     *       position; the original element at `to_remove` is overwritten.
     * @note The function does not update or return a new length and does not
     *       free or reallocate `data`. The last slot (index `len-1`) will
     *       contain a moved-from or overwritten object; the caller must handle
     *       destruction or size bookkeeping as appropriate.
     * @complexity Linear in the number of elements moved: O(len - to_remove).
     */
    template <typename T>
    void Reindex(
        T *&data,
        const Trait::VeryLarge &len,
        const Trait::VeryLarge &to_remove
    )
    {
        if (to_remove >= len)
        {
            throw Except::OutOfRange();
        }

        // Edge case: len = 1 and to_remove = 0
        if (len == 1 && to_remove == 0)
        {
            // If T is not trivially destructible, call the destructor
            if constexpr (!std::is_trivially_destructible_v<T>)
            {
                // Explicitly destroy the single element if needed
                data[0].~T();
            }

            // No shifting needed; single element removed
            return;
        }

        // Shift elements left to overwrite the removed index
        for (Trait::VeryLarge i = to_remove; i < len - 1; ++i)
        {
            if constexpr (std::is_trivially_copyable_v<T>)
            {
                // Move directly for trivially copyable types
                data[i] = data[i + 1];
            }
            // Check for move constructibility
            else if constexpr (std::is_move_assignable_v<T>)
            {
                // Use move assignment for move-assignable types
                data[i] = std::move(data[i + 1]);
            }
            else
            {
                // Fallback to move-construction into existing storage,
                // then explicitly destroy the moved-from element.
                new (&data[i]) T(std::move(data[i + 1]));
                data[i + 1].~T();
            }
        }
    }
}
