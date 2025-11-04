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
#include "celery/list/node.h"

namespace Celery::List
{
    /**
     * @brief Iterator for a linked list.
     *
     * Provides basic iteration capabilities over a linked list
     * by maintaining a pointer to the current node.
     *
     * @tparam T The type of data stored in the linked list nodes.
     */
    template <typename T>
    class Iterator
    {
    protected:
        // Pointer to the current node in the linked list
        Internal::LinkedListNode<T> *current;

    public:
        /**
         * @brief Construct an Iterator starting at the given node.
         *
         * @param start Pointer to the starting node of the iteration.
         */
        Iterator(Internal::LinkedListNode<T> *start) : current(start)
        {}

        /**
         * @brief Dereference the iterator to access the current node's data.
         *
         * @return Reference to the data of the current node.
         */
        T &operator*()
        {
            return current->data;
        }

        /**
         * @brief Advance the iterator to the next node.
         *
         * @return Reference to the updated iterator.
         */
        Iterator &operator++()
        {
            if (current)
            {
                current = current->next;
            }
            return *this;
        }

        /**
         * @brief Compare this iterator with another for equality.
         *
         * @param other The other iterator to compare against.
         * @return true if both iterators point to the same node, false otherwise.
         */
        bool operator!=(const Iterator &other) const
        {
            return current != other.current;
        }
    };
}