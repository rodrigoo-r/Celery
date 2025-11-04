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

namespace Celery::List::Internal
{
    /**
     * @brief Node structure for a doubly linked list.
     *
     * Each node contains pointers to the next and previous nodes,
     * as well as the data of type T.
     *
     * @tparam T The type of data stored in the node.
     */
    template<typename T>
    struct LinkedListNode
    {
        LinkedListNode *next = nullptr;
        LinkedListNode *prev = nullptr;
        T data;
    };
}