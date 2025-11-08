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
#include <utility>

#include "celery/base/comparable.h"
#include "celery/memory/monotonic.h"

namespace Celery::Tree
{
    namespace Pmr
    {
        /**
         * @brief Node type used by the Red-Black tree.
         *
         * Template parameter T is the stored value type.
         * This structure contains pointers for parent and children
         * and a boolean indicating the color (true = red, false = black).
         */
        template<typename T>
        struct RedBlackNode
        {
            T data;                         ///< The data stored in the node.
            bool red = true;                ///< Node color: `true` == red, `false` == black.
            RedBlackNode *left = nullptr;   ///< Pointer to left child.
            RedBlackNode *right = nullptr;  ///< Pointer to right child.
            RedBlackNode *parent = nullptr; ///< Pointer to parent node.
        };

        /**
         * @brief In-order iterator for the Red-Black tree.
         *
         * Provides pre/post increment and decrement to traverse tree in sorted order.
         * Iterator holds a raw pointer to a `RedBlackNode<T>`.
         *
         * @tparam T Value type stored in nodes.
         */
        template<typename T>
        class RedBlackIterator
        {
        protected:
            using Ptr = T*;
            using NodeType = RedBlackNode<T>*;
            using Reference = T&;

        public:
            /**
             * @brief Construct iterator from a node pointer.
             * @param node Pointer to starting node (default nullptr).
             */
            RedBlackIterator(NodeType node = nullptr) : node_(node) {}

            /**
             * @brief Dereference operator returning reference to stored value.
             * @note Behavior is undefined if iterator is null.
             */
            Reference operator*() const { return node_->data; }

            /**
             * @brief Member access operator.
             * @note Behavior is undefined if iterator is null.
             */
            Ptr operator->() const { return &(node_->data); }

            /**
             * @brief Pre-increment: move to next element in in-order traversal.
             * @return Reference to advanced iterator.
             */
            RedBlackIterator& operator++()
            {
                if (node_->right)
                {
                    // Next is leftmost node of right subtree.
                    node_ = node_->right;
                    while (node_->left)
                        node_ = node_->left;
                }
                else
                {
                    // Walk up until we come from left child.
                    NodeType parent = node_->parent;
                    while (parent && node_ == parent->right)
                    {
                        node_ = parent;
                        parent = parent->parent;
                    }
                    node_ = parent;
                }
                return *this;
            }

            /**
             * @brief Post-increment: returns copy before incrementing.
             */
            RedBlackIterator operator++(int)
            {
                RedBlackIterator tmp = *this;
                ++(*this);
                return tmp;
            }

            /**
             * @brief Pre-decrement: move to previous element in in-order traversal.
             * @return Reference to decremented iterator.
             */
            RedBlackIterator& operator--()
            {
                if (node_->left)
                {
                    // Previous is rightmost node of left subtree.
                    node_ = node_->left;
                    while (node_->right)
                        node_ = node_->right;
                }
                else
                {
                    // Walk up until we come from right child.
                    NodeType parent = node_->parent;
                    while (parent && node_ == parent->left)
                    {
                        node_ = parent;
                        parent = parent->parent;
                    }
                    node_ = parent;
                }
                return *this;
            }

            /**
             * @brief Post-decrement: returns copy before decrementing.
             */
            RedBlackIterator operator--(int)
            {
                RedBlackIterator tmp = *this;
                --(*this);
                return tmp;
            }

            /**
             * @brief Equality comparison.
             */
            bool operator==(const RedBlackIterator& other) const { return node_ == other.node_; }

            /**
             * @brief Inequality comparison.
             */
            bool operator!=(const RedBlackIterator& other) const { return node_ != other.node_; }

            /**
             * @brief Access underlying node pointer.
             * @return Raw node pointer.
             */
            NodeType node() const { return node_; }

        private:
            NodeType node_; ///< Underlying node pointer.
        };

        /**
         * @brief Red-Black tree container with minimal STL-like interface.
         *
         * Uses a Policy-based allocator (default is `MonotonicAllocator<RedBlackNode<T>>`).
         * SFINAE ensures supplied Allocator derives from `Celery::Pmr::Allocator<RedBlackNode<T>>`.
         *
         * Inherits from helper base classes to provide size, push and remove semantics.
         *
         * @tparam T Value type stored in the tree.
         * @tparam Allocator Allocator type for node allocation/deallocation.
         */
        template<
            typename T,
            typename Allocator = Celery::Pmr::MonotonicAllocator<RedBlackNode<T>>,
            typename EqCompare = Base::EqualityCompare<T>,
            typename ArithCompare = Base::ArithmeticCompare<T>,
            // SFINAE to ensure Allocator is a valid allocator
            typename = Trait::EnsureAllocator<Allocator>,
            typename = Trait::EnsureCompare<EqCompare>,
            typename = Trait::EnsureArithmeticCompare<ArithCompare>
        >
        class RedBlack :
            public Base::Sizeable,
            public Base::Removable<RedBlack<T, Allocator>, T>,
            public Base::DirectlyPushable<RedBlack<T, Allocator>, T>
        {
        protected:
            using NodeType = RedBlackNode<T> *;
            NodeType root = nullptr; // Root of the red-black tree

            /**
             * @brief Left rotate subtree rooted at x.
             *
             * Performs standard BST left-rotation and updates parent/child pointers.
             * @param x Root of subtree to rotate.
             */
            void RotateLeft(NodeType x)
            {
                NodeType y = x->right;
                x->right = y->left;
                if (y->left != nullptr)
                {
                    y->left->parent = x;
                }
                y->parent = x->parent;
                if (x->parent == nullptr)
                {
                    root = y;
                }
                else if (x == x->parent->left)
                {
                    x->parent->left = y;
                }
                else
                {
                    x->parent->right = y;
                }
                y->left = x;
                x->parent = y;
            }

            /**
             * @brief Right rotate subtree rooted at y.
             *
             * Performs standard BST right-rotation and updates parent/child pointers.
             * @param y Root of subtree to rotate.
             */
            void RotateRight(NodeType y)
            {
                NodeType x = y->left;
                y->left = x->right;
                if (x->right != nullptr)
                {
                    x->right->parent = y;
                }
                x->parent = y->parent;
                if (y->parent == nullptr)
                {
                    root = x;
                }
                else if (y == y->parent->right)
                {
                    y->parent->right = x;
                }
                else
                {
                    y->parent->left = x;
                }
                x->right = y;
                y->parent = x;
            }

            /**
             * @brief Restore red-black properties after insertion.
             *
             * Walks up the tree and fixes coloring and performs rotations
             * as required to maintain red-black invariants.
             *
             * @param ptr Newly inserted node that may violate properties.
             */
            void FixInsert(NodeType ptr)
            {
                NodeType parent = nullptr;
                NodeType grandparent = nullptr;

                while (
                    ptr != root &&
                    ptr->red &&
                    ptr->parent->red
                )
                {
                    parent = ptr->parent;
                    grandparent = parent->parent;

                    // Parent is left child of grandparent.
                    if (parent == grandparent->left)
                    {
                        NodeType uncle = grandparent->right;

                        // Case 1: Uncle is red -> recolor
                        if (uncle != nullptr && uncle->red)
                        {
                            grandparent->red = true;
                            parent->red = false;
                            uncle->red = false;
                            ptr = grandparent;
                        } else
                        {
                            // Case 2: ptr is right child -> rotate left at parent
                            if (ptr == parent->right)
                            {
                                RotateLeft(parent);
                                ptr = parent;
                                parent = ptr->parent;
                            }
                            // Case 3: ptr is left child -> rotate right at grandparent
                            RotateRight(grandparent);
                            std::swap(parent->red, grandparent->red);
                            ptr = parent;
                        }
                    }
                    // Parent is right child of grandparent.
                    else {
                        NodeType uncle = grandparent->left;

                        if (uncle != nullptr && uncle->red)
                        {
                            grandparent->red = true;
                            parent->red = false;
                            uncle->red = false;
                            ptr = grandparent;
                        } else {
                            if (ptr == parent->left)
                            {
                                RotateRight(parent);
                                ptr = parent;
                                parent = ptr->parent;
                            }
                            RotateLeft(grandparent);
                            std::swap(parent->red, grandparent->red);
                            ptr = parent;
                        }
                    }
                }

                // Ensure root is black after fixes.
                if (root) root->red = false;
            }

            /**
             * @brief Restore red-black properties after deletion.
             *
             * Implements the fix-up algorithm for deletion which may require
             * rotations and recoloring as it walks up the tree.
             *
             * @param pt Node from which to start fix-up (may be nullptr).
             */
            void FixDelete(NodeType pt)
            {
                NodeType parent = nullptr;
                NodeType grandparent = nullptr;

                // Traverse up and fix double-black situations.
                while (pt != root && pt->red && pt->parent->red)
                {
                    parent = pt->parent;
                    grandparent = parent->parent;

                    // Parent is left child
                    if (parent == grandparent->left)
                    {
                        NodeType uncle = grandparent->right;

                        if (uncle != nullptr && uncle->red)
                        {
                            grandparent->red = true;
                            parent->red = false;
                            uncle->red = false;
                            pt = grandparent;
                        }
                        else
                        {
                            if (pt == parent->right)
                            {
                                RotateLeft(parent);
                                pt = parent;
                                parent = pt->parent;
                            }

                            RotateRight(grandparent);
                            std::swap(parent->red, grandparent->red);
                            pt = parent;
                        }
                    }
                    // Parent is a right child
                    else
                    {
                        NodeType uncle = grandparent->left;

                        if (uncle != nullptr && uncle->red)
                        {
                            grandparent->red = true;
                            parent->red = false;
                            uncle->red = false;
                            pt = grandparent;
                        }
                        else
                        {
                            if (pt == parent->left)
                            {
                                RotateRight(parent);
                                pt = parent;
                                parent = pt->parent;
                            }

                            RotateLeft(grandparent);
                            std::swap(parent->red, grandparent->red);
                            pt = parent;
                        }
                    }
                }

                // Ensure the root is always black.
                if (root) root->red = false;
            }

            /**
             * @brief Find minimum (left-most) node in subtree.
             * @param node Root of subtree.
             * @return Pointer to minimum node (never null when called with non-null).
             */
            NodeType Minimum(NodeType node)
            {
                while (node->left != nullptr)
                {
                    node = node->left;
                }
                return node;
            }

            /**
             * @brief Replace subtree rooted at u with subtree rooted at v.
             *
             * Maintains parent pointers accordingly. Used during deletion.
             *
             * @param u Node being replaced.
             * @param v Node that replaces u (may be nullptr).
             */
            void Transplant(NodeType u, NodeType v)
            {
                if (u->parent == nullptr)
                {
                    root = v;
                }
                else if (u == u->parent->left)
                {
                    u->parent->left = v;
                }
                else
                {
                    u->parent->right = v;
                }
                if (v != nullptr)
                {
                    v->parent = u->parent;
                }
            }

            /**
             * @brief Locate the first node with value equal to `value`.
             *
             * Traverses the tree using equality and ordering comparisons
             * to find the node containing `value`. Returns nullptr if not found.
             *
             * @param value Value to locate.
             * @return Pointer to node containing value, or nullptr if not found.
             */
            template <
                class U = T,
                typename = Trait::EnsureSame<T, U>
            >
            NodeType Locate(U &&value)
            {
                NodeType current = root;
                while (current != nullptr)
                {
                    if (Base::EqualityCompare<T>::Eq(current->data, value))
                    {
                        return current;
                    }

                    if (Base::ArithmeticCompare<T>::Lt(value, current->data))
                    {
                        current = current->left;
                    }
                    else
                    {
                        current = current->right;
                    }
                }
                return nullptr; // Not found
            }

            template<typename ...Args>
            NodeType Insert(Args &&...args)
            {
                // Allocate and construct a node using allocator policy.
                NodeType new_node = Allocator::Allocate(std::forward<Args>(args)...);
                NodeType parent = nullptr;
                NodeType current = root;

                // Find the correct position to insert the new node (BST insert).
                while (current != nullptr)
                {
                    parent = current;
                    if (Base::ArithmeticCompare<T>::Lt(new_node->data, current->data))
                    {
                        current = current->left;
                    }
                    else if (Base::ArithmeticCompare<T>::Lt(current->data, new_node->data))
                    {
                        current = current->right;
                    }
                    else
                    {
                        // Prevent duplicates: deallocate and return.
                        Allocator::Deallocate(new_node);
                        return current; // Duplicate found
                    }
                }

                // Attach new node to parent and set its parent pointer.
                new_node->parent = parent;

                // Insert node as root if tree was empty.
                if (parent == nullptr)
                {
                    root = new_node; // Tree was empty
                    root = new_node; // Duplicate line preserved from original code.
                }
                else if (Base::ArithmeticCompare<T>::Lt(new_node->data, parent->data))
                {
                    parent->left = new_node;
                }
                else
                {
                    parent->right = new_node;
                }

                // Restore red-black properties after insertion.
                FixInsert(new_node);
                ++this->len; // Update size counter inherited from Base::Sizeable.

                return new_node; // Return pointer to inserted node.
            }

            /**
             * @brief Remove the specified node from the tree.
             *
             * Performs standard BST deletion with transplant and minimum replacement,
             * updates colors, deallocates the node using Allocator::Deallocate, and fixes
             * tree properties using FixDelete when required.
             *
             * If `z` is nullptr, function returns silently.
             *
             * @param z Node to remove.
             */
            void RemoveBase(NodeType z)
            {
                NodeType x, y;
                if (z == nullptr)
                {
                    return; // Value not found, nothing to remove.
                }

                // Save original node/color for fix-up decisions.
                y = z;
                bool y_original_color = y->red;
                if (z->left == nullptr)
                {
                    x = z->right;
                    Transplant(z, z->right);
                }
                else if (z->right == nullptr)
                {
                    x = z->left;
                    Transplant(z, z->left);
                }
                else
                {
                    // Node has two children: find in-order successor and replace.
                    y = Minimum(z->right);
                    y_original_color = y->red;
                    x = y->right;
                    if (y->parent == z)
                    {
                        if (x != nullptr)
                        {
                            x->parent = y;
                        }
                    }
                    else
                    {
                        Transplant(y, y->right);
                        y->right = z->right;
                        y->right->parent = y;
                    }
                    Transplant(z, y);
                    y->left = z->left;
                    y->left->parent = y;
                    y->red = z->red;
                }

                // Deallocate the removed node via supplied allocator policy.
                Allocator::Deallocate(z);

                // If the removed node was black, we may have broken properties.
                if (!y_original_color)
                {
                    FixDelete(x);
                }
                --this->len; // Update container size.
            }

        public:
            /**
             * @brief Construct an empty Red-Black tree.
             *
             * Compile-time check verifies T has a Comparable specialization.
             */
            RedBlack()
            {
                root = nullptr;

                // Ensure T is comparable at compile-time.
                static_assert(
                    Base::Comparable<T> || (
                        Base::DefaultEqualityComparable<T> &&
                        Base::DefaultArithmeticComparable<T>
                    ),
                    "RedBlack tree requires comparable type T. "
                    "Please specialize Celery::Base::Comparable for your type."
                );
            }

            /**
             * @brief Construct a new node in-place and insert it into the tree.
             *
             * Uses Allocator::Allocate to build a new node. The new node is inserted
             * following BST insertion rules and then FixInsert is called to maintain
             * red-black invariants.
             *
             * @tparam Args Argument pack forwarded to allocator/node constructor.
             * @param args Arguments forwarded to construct the node's T.
             */
            template<typename ...Args>
            void EmplaceBack(Args &&...args)
            {
                // Delegate to internal Insert method.
                Insert(std::forward<Args>(args)...);
            }

            /**
             * @brief Remove first node whose value equals `value`.
             *
             * SFINAE ensures the provided `U` matches `T` decay-wise. Finds the node,
             * performs the standard BST deletion with transplant and minimum replacement,
             * updates colors, deallocates the node using Allocator::Deallocate, and fixes
             * tree properties using FixDelete when required.
             *
             * If value not found, function returns silently.
             *
             * @tparam U Decay-compatible type for value lookup.
             * @param value Value to remove.
             */
            template<
                class U = T,
                typename = Trait::EnsureSame<T, U>
            >
            void Remove(U &&value)
            {
                RemoveBase(Locate(std::forward<U>(value))); // Locate and remove the node.
            }

            /**
             * @brief Return iterator to smallest element (begin).
             * @return RedBlackIterator<T> pointing to minimum node, or null iterator if empty.
             */
            RedBlackIterator<T> begin()
            {
                NodeType node = root;
                if (!node)
                    return { nullptr };
                while (node->left)
                    node = node->left;
                return { node };
            }

            /**
             * @brief Check if tree contains a value equal to `value`.
             *
             * SFINAE ensures the provided `U` matches `T` decay-wise. Uses Locate
             * to find the node and returns true if found, false otherwise.
             *
             * @tparam U Decay-compatible type for value lookup.
             * @param value Value to check for containment.
             * @return true if value is in tree, false otherwise.
             */
            template <
                class U = T,
                typename = Trait::EnsureSame<T, U>
            >
            bool Contains(U &&value)
            {
                return Locate(std::forward<U>(value)) != nullptr;
            }

            /**
             * @brief Return end iterator (null).
             * @return Null iterator representing end.
             */
            RedBlackIterator<T> end()
            {
                return { nullptr };
            }
        };
    }

    template <typename T>
    using RedBlack = Pmr::RedBlack<T>;
}
