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
#include "celery/except/out_of_range.h"
#include "celery/base/sizeable.h"
#include "celery/memory/system.h"
#include "iterator.h"
#include "node.h"

namespace Celery::List
{
    namespace Pmr
    {
        /**
         * @brief A doubly-linked list implementation that uses a polymorphic memory resource allocator.
         *
         * This class is an STL-like container providing basic list operations
         * such as push/pop at both ends, removal by value or index, iteration,
         * and clearing. It stores nodes of type Celery::List::Internal::LinkedListNode<T>
         * and uses the provided Allocator for node allocation and deallocation.
         *
         * @tparam T The element type stored in the list. Must be copy-constructible.
         * @tparam Allocator The allocator type used to allocate/deallocate nodes.
         *         Defaults to Celery::Pmr::Allocator<Internal::LinkedListNode<T>>.
         *
         * @note SFINAE constraints enforce that T is copy-constructible and that
         *       Allocator derives from the expected allocator base.
         */
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
            /** @brief Pointer to the first node in the list (or nullptr if empty). */
            Internal::LinkedListNode<T> *head = nullptr;

            /** @brief Pointer to the last node in the list (or nullptr if empty). */
            Internal::LinkedListNode<T> *tail = nullptr;

            /**
             * @brief Remove and deallocate a node from the list, updating links and size.
             *
             * This method relinks adjacent nodes (previous and next), updates the
             * head/tail pointers when removing at boundaries, deallocates the node
             * through the Allocator, and decrements the container size.
             *
             * @param node Pointer to the node to remove. Must not be nullptr.
             *
             * @throws Except::OutOfRange If `node` is nullptr.
             */
            T RemoveNode(Internal::LinkedListNode<T> *node)
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

                // Move the data out before deallocation
                T value = std::move(node->data);
                Allocator::Deallocate(node);
                --this->len;

                // Transfer ownership to caller
                return value;
            }

        public:
            /**
             * @brief Construct and append a new element at the end of the list.
             *
             * Forwards arguments to the node allocator which constructs the element in-place.
             *
             * @tparam Args Parameter pack for element construction.
             * @param args Arguments forwarded to T's constructor.
             *
             * Complexity: constant time.
             */
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

            /**
             * @brief Construct and insert a new element at the front of the list.
             *
             * Forwards arguments to the node allocator which constructs the element in-place.
             *
             * @tparam Args Parameter pack for element construction.
             * @param args Arguments forwarded to T's constructor.
             *
             * Complexity: constant time.
             */
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

            /**
             * @brief Access the element at the specified index.
             *
             * Traverses the list from the head to locate the node at position `index`
             * and returns a reference to its data.
             *
             * @tparam U Unsigned integral index type (defaults to Trait::Uint).
             * @param index Index of the element to access (0-based).
             *
             * @throws Except::OutOfRange If `index` is greater than or equal to the current size.
             *
             * Complexity: linear in `index` (worst-case linear in list size).
             *
             * @return Reference to the element at the specified index.
             */
            template <
                class U = Trait::Uint,
                typename = std::enable_if_t<
                    std::is_integral_v<U>
                >
            >
            T &operator[](U &&index)
            {
                // Check bounds
                if (index >= this->len)
                {
                    throw Except::OutOfRange();
                }

                // Traverse to the node at index
                Internal::LinkedListNode<T> *current = head;
                for (Trait::Uint i = 0; i < index; ++i)
                {
                    current = current->next;
                }

                return current->data;
            }

            /**
             * @brief Remove and return the last element of the list.
             *
             * @throws Except::OutOfRange If the list is empty.
             *
             * Complexity: constant time.
             *
             * @return The removed element.
             * @note Ownership of the returned element is transferred to the caller.
             */
            T PopBackMove()
            {
                if (!tail)
                {
                    throw Except::OutOfRange();
                }

                return std::move(RemoveNode(tail));
            }

            /**
             * @brief Remove and return the first element of the list.
             *
             * @throws Except::OutOfRange If the list is empty.
             *
             * Complexity: constant time.
             *
             * @return The removed element.
             * @note Ownership of the returned element is transferred to the caller.
             */
            T PopFrontMove()
            {
                if (!head)
                {
                    throw Except::OutOfRange();
                }

                return std::move(RemoveNode(head));
            }

            /**
             * @brief Remove the first element of the list.
             *
             * @throws Except::OutOfRange If the list is empty.
             *
             * Complexity: constant time.
             */
            void PopFront()
            {
                PopFrontMove();
            }

            /**
             * @brief Remove the last element of the list.
             *
             * @throws Except::OutOfRange If the list is empty.
             *
             * Complexity: constant time.
             */
            void PopBack()
            {
                PopBackMove();
            }

            /**
             * @brief Remove all elements from the list.
             *
             * After this call the list is empty and size is zero.
             *
             * Complexity: linear.
             */
            void Clear()
            {
                while (this->len > 0)
                {
                    PopFront();
                }

                this->len = 0;
            }

            /**
             * @brief Insert a value at the front of the list by forwarding the provided value.
             *
             * @tparam U Type of the value being inserted (deduced).
             * @param value Value to insert (forwarded).
             */
            template <class U = T>
            void PushFront(U &&value)
            {
                EmplaceFront(std::forward<U>(value));
            }

            /**
             * @brief Remove the element at the specified index.
             *
             * Traverses from the head to locate the node at position `idx` and removes it.
             *
             * @tparam U Unsigned integral index type (defaults to Trait::Uint).
             * @param idx Index of the element to remove (0-based).
             *
             * @throws Except::OutOfRange If `idx` is greater than or equal to the current size.
             *
             * Complexity: linear in `idx` (worst-case linear in list size).
             */
            template <
                class U = Trait::Uint,
                typename = std::enable_if_t<
                    std::is_integral_v<U>
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

            /**
             * @brief Remove the first occurrence of a value equal to `value`.
             *
             * Traverses the list and compares elements using `operator==`. If a match
             * is found, the node is removed and the function returns.
             *
             * @tparam U Type of the value to remove (deduced).
             * @param value Value to remove.
             *
             * Complexity: linear in list size (stops at first match).
             */
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

            /**
             * @brief Return an iterator to the beginning of the list.
             *
             * @return Iterator pointing to the first element (or `end()` if empty).
             */
            Iterator<T> begin()
            {
                return { head };
            }

            /**
             * @brief Return an iterator representing the end of the list.
             *
             * @return Iterator representing one-past-the-last element (nullptr).
             */
            Iterator<T> end()
            {
                return { nullptr };
            }

            /**
             * @brief Destructor: clean up all nodes and release resources.
             *
             * Ensures all nodes are removed and deallocated via the Allocator.
             */
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
