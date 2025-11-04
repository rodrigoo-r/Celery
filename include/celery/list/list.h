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
#include "celery/base/sizeable.h"
#include "celery/memory/system.h"
#include "iterator.h"
#include "node.h"

namespace Celery::List
{
    namespace Pmr
    {
        template<
            typename T,
            typename Allocator = Celery::Pmr::Allocator<Internal::LinkedListNode<T>>,
            // SFINAE to ensure T is copy-constructible
            typename = std::enable_if_t<
                std::is_copy_constructible_v<T>
            >,
            typename = std::enable_if_t<
                std::is_base_of_v<
                    Celery::Pmr::Allocator<Internal::LinkedListNode<T>>,
                    Allocator
                >
            >
        >
        class LinkedList :
            public Base::Sizeable,
            public Base::DirectlyPushable<LinkedList<T, Allocator>, T>,
            public Base::Removable<LinkedList<T, Allocator>, T>
        {
        protected:
            Internal::LinkedListNode<T> *head = nullptr;
            Internal::LinkedListNode<T> *tail = nullptr;

            void RemoveNode(Internal::LinkedListNode<T> *node)
            {
                if (!node)
                {
                    throw Except::OutOfRange();
                }

                // Re-link the previous and next nodes
                if (node->prev)
                {
                    node->prev->next = node->next;
                }
                else
                {
                    head = node->next; // Update head if needed
                }

                if (node->next)
                {
                    node->next->prev = node->prev;
                }
                else
                {
                    tail = node->prev; // Update tail if needed
                }

                Allocator::Deallocate(node);
                --this->len;
            }

        public:
            template<typename... Args>
            void EmplaceBack(Args&&... args)
            {
                Internal::LinkedListNode<T> *new_node = Allocator::Allocate(
                    nullptr,
                    nullptr,
                    std::forward<Args>(args)...
                );

                // Case 1: List is not empty
                if (tail)
                {
                    // Append to the end
                    tail->next = new_node;
                    new_node->prev = tail;
                    tail = new_node;
                }
                // Case 2: List is empty
                else
                {
                    head = tail = new_node;
                }

                ++this->len;
            }

            template<typename... Args>
            void EmplaceFront(Args&&... args)
            {
                Internal::LinkedListNode<T> *new_node = Allocator::Allocate(
                    nullptr,
                    nullptr,
                    std::forward<Args>(args)...
                );

                // Case 1: List is not empty
                if (head)
                {
                    // Insert at the front
                    head->prev = new_node;
                    new_node->next = head;
                    head = new_node;
                }
                // Case 2: List is empty
                else
                {
                    head = tail = new_node;
                }

                ++this->len;
            }

            void PopBack()
            {
                if (!tail)
                {
                    throw Except::OutOfRange();
                }

                RemoveNode(tail);
            }

            void PopFront()
            {
                if (!head)
                {
                    throw Except::OutOfRange();
                }

                RemoveNode(head);
            }

            void Clear()
            {
                while (this->len > 0)
                {
                    PopFront();
                }

                this->len = 0;
            }

            template <class U = T>
            void PushFront(U &&value)
            {
                EmplaceFront(std::forward<U>(value));
            }

            template <
                class U = Trait::Uint,
                typename = std::enable_if_t<
                    std::is_integral_v<U> &&
                    std::is_unsigned_v<U>
                >
            >
            void RemoveAt(U &&idx)
            {
                // Check bounds
                if (idx >= this->len)
                {
                    throw Except::OutOfRange();
                }

                // Traverse to the node at idx
                Internal::LinkedListNode<T> *current = head;
                for (U i = 0; i < idx; ++i)
                {
                    current = current->next;
                }

                // Remove the found node
                RemoveNode(current);
            }

            template <typename U = T>
            void Remove(U &&value)
            {
                // Traverse the list to find the value
                Internal::LinkedListNode<T> *current = head;
                while (current)
                {
                    if (current->data == value)
                    {
                        // Found the node to remove
                        RemoveNode(current);
                        return; // Remove only the first occurrence
                    }

                    current = current->next;
                }
            }

            Iterator<T> begin()
            {
                return { head };
            }

            Iterator<T> end()
            {
                return { nullptr };
            }

            ~LinkedList()
            override {
                // Clean up all nodes
                Clear();
            }
        };
    }

    template <typename T>
    using LinkedList = Pmr::LinkedList<T>;
}