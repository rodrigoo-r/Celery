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
#include "celery/memory/monotonic.h"
#include "celery/misc/pair.h"
#include "celery/tree/rb.h"

namespace Celery::Collection
{
    /*
     * @brief Specialization of EqualityCompare for Misc::Pair.
     *
     * This specialization provides equality comparison operations
     * for the Misc::Pair container by comparing only the Key component.
     *
     * @tparam Key Type of the key in the pair.
     * @tparam Value Type of the value in the pair.
     */
    template<typename Key, typename Value>
    struct MapEqualityCompare :
        Base::EqualityCompare<Misc::Pair<Key, Value>>
    {
        using Element = Misc::Pair<Key, Value>;

        /*
         * @brief Compare two Pair elements for equality based on their keys.
         *
         * @param a First Pair element to compare.
         * @param b Second Pair element to compare.
         * @return true if the keys of a and b are equal, false otherwise.
         */
        template<
            class U = Element,
            typename = Trait::EnsureSame<U, Element>
        >
        static bool Eq(U&& a, U&& b)
        {
            // Detect if operator== exists for Key
            if constexpr (Base::Comparable<Key> || Base::DefaultEqualityComparable<Key>)
            {
                return Base::EqualityCompare<Key>::Eq(
                    std::forward<Key>(a.First()),
                    std::forward<Key>(b.First())
                );
            } else
            {
                static_assert(
                    false,
                    "Key type must support operator== or be Comparable for Pair comparison."
                );

                // Unreachable, but required to satisfy all control paths
                return false;
            }
        }

        /*
         * @brief Compare two Pair elements for inequality based on their keys.
         *
         * @param a First Pair element to compare.
         * @param b Second Pair element to compare.
         * @return true if the keys of a and b are not equal, false otherwise.
         */
        template<
            class U = Element,
            typename = Trait::EnsureSame<U, Element>
        >
        static bool Neq(U&& a, U&& b)
        {
            // Detect if operator!= exists for Key
            if constexpr (Base::Comparable<Key> || Base::DefaultEqualityComparable<Key>)
            {
                return Base::EqualityCompare<Key>::Neq(
                    std::forward<Key>(a.First()),
                    std::forward<Key>(b.First())
                );
            } else
            {
                static_assert(
                    false,
                    "Key type must support operator!= or be Comparable for Pair comparison."
                );

                // Unreachable, but required to satisfy all control paths
                return false;
            }
        }
    };

    /*
     * @brief Specialization of ArithmeticCompare for Misc::Pair.
     *
     * This specialization provides arithmetic comparison operations
     * for the Misc::Pair container by comparing only the Key component.
     *
     * @tparam Key Type of the key in the pair.
     * @tparam Value Type of the value in the pair.
     */
    template<typename Key, typename Value>
    struct MapArithmeticCompare :
        Base::ArithmeticCompare<Misc::Pair<Key, Value>>
    {
        using Element = Misc::Pair<Key, Value>;

        /*
         * @brief Compare two Pair objects for less-than based on their keys.
         *
         * This method compares the `First` elements (keys) of the two Pair objects
         * using the ArithmeticCompare specialization for the Key type.
         *
         * @param a First Pair object to compare.
         * @param b Second Pair object to compare.
         * @return true if the key of `a` is less than the key of `b`, false otherwise.
         */
        template<
            class U = Element,
            typename = Trait::EnsureSame<U, Element>
        >
        static bool Lt(U&& a, U&& b)
        {
            // Detect if operator< exists for Key
            if constexpr (Base::ArithmeticComparable<Key> || Base::DefaultArithmeticComparable<Key>)
            {
                return Base::ArithmeticCompare<Key>::Lt(
                    std::forward<Key>(a.First()),
                    std::forward<Key>(b.First())
                );
            } else
            {
                static_assert(
                    false,
                    "Key type must support operator< or be Comparable for Pair comparison."
                );

                // Unreachable, but required to satisfy all control paths
                return false;
            }
        }

        /*
         * @brief Compare two Pair objects for less-than-or-equal based on their keys.
         *
         * This method compares the `First` elements (keys) of the two Pair objects
         * using the ArithmeticCompare specialization for the Key type.
         *
         * @param a First Pair object to compare.
         * @param b Second Pair object to compare.
         * @return true if the key of `a` is less than or equal to the key of `b`, false otherwise.
         */
        template<
            class U = Element,
            typename = Trait::EnsureSame<U, Element>
        >
        static bool Gt(U&& a, U&& b)
        {
            // Detect if operator> exists for Key
            if constexpr (Base::ArithmeticComparable<Key> || Base::DefaultArithmeticComparable<Key>)
            {
                return Base::ArithmeticCompare<Key>::Gt(
                    std::forward<Key>(a.First()),
                    std::forward<Key>(b.First())
                );
            } else
            {
                static_assert(
                    false,
                    "Key type must support operator> or be Comparable for Pair comparison."
                );

                // Unreachable, but required to satisfy all control paths
                return false;
            }
        }

        /*
         * @brief Compare two Pair objects for greater-than based on their keys.
         *
         * This method compares the `First` elements (keys) of the two Pair objects
         * using the ArithmeticCompare specialization for the Key type.
         *
         * @param a First Pair object to compare.
         * @param b Second Pair object to compare.
         * @return true if the key of `a` is greater than the key of `b`, false otherwise.
         */
        template<
            class U = Element,
            typename = Trait::EnsureSame<U, Element>
        >
        static bool Gte(U&& a, U&& b)
        {
            // Detect if operator>= exists for Key
            if constexpr (Base::ArithmeticComparable<Key> || Base::DefaultArithmeticComparable<Key>)
            {
                return Base::ArithmeticCompare<Key>::Gt(
                    std::forward<Key>(a.First()),
                    std::forward<Key>(b.First())
                );
            } else
            {
                static_assert(
                    false,
                    "Key type must support operator>= or be Comparable for Pair comparison."
                );

                // Unreachable, but required to satisfy all control paths
                return false;
            }
        }

        /*
         * @brief Compare two Pair objects for less-than-or-equal based on their keys.
         *
         * This method compares the `First` elements (keys) of the two Pair objects
         * using the ArithmeticCompare specialization for the Key type.
         *
         * @param a First Pair object to compare.
         * @param b Second Pair object to compare.
         * @return true if the key of `a` is less than or equal to the key of `b`, false otherwise.
         */
        template<
            class U = Element,
            typename = Trait::EnsureSame<U, Element>
        >
        static bool Lte(U&& a, U&& b)
        {
            // Detect if operator<= exists for Key
            if constexpr (Base::ArithmeticComparable<Key> || Base::DefaultArithmeticComparable<Key>)
            {
                return Base::ArithmeticCompare<Key>::Lte(
                    std::forward<Key>(a.First()),
                    std::forward<Key>(b.First())
                );
            } else
            {
                static_assert(
                    false,
                    "Key type must support operator<= or be Comparable for Pair comparison."
                );

                // Unreachable, but required to satisfy all control paths
                return false;
            }
        }
    };

    /*
     * @brief PMR-enabled Map container.
     *
     * This class implements a Map using a Red-Black Tree as the underlying
     * data structure. It stores key-value pairs using the Misc::Pair container.
     * The Map supports custom PMR-style allocators for memory management.
     *
     * @tparam Key Type of the keys in the map.
     * @tparam Value Type of the values in the map.
     * @tparam Allocator Allocator type used for memory management.
     *                   Defaults to MonotonicAllocator for RedBlackNode<Pair<Key, Value>>.
     */
    namespace Pmr
    {
        template<
            typename Key,
            typename Value,
            typename EqCompare = MapEqualityCompare<Key, Value>,
            typename ArithCompare = MapArithmeticCompare<Key, Value>,
            Trait::Decimal CleanupGrowthFactor = Trait::GrowthFactor,
            Trait::Uint CleanupGrowthInitialCapacity = Trait::InitialCapacity,
            typename Allocator = Celery::Pmr::MonotonicAllocator<
                Tree::Pmr::RedBlackNode<Misc::Pair<Key, Value>>
            >,
            typename CleanupAllocator = Celery::Pmr::ArrayAllocator<
                Tree::Pmr::RedBlackNode<Misc::Pair<Key, Value>> *
            >
        >
        class Map :
            public Tree::Pmr::RedBlack<
                Misc::Pair<Key, Value>,
                EqCompare,
                ArithCompare,
                CleanupGrowthFactor,
                CleanupGrowthInitialCapacity,
                Allocator,
                CleanupAllocator
            >
        {
        protected:
            using PairType = Misc::Pair<Key, Value>;
            using NodeType = Tree::Pmr::RedBlackNode<PairType> *;

            /*
             * @brief Helper method to emplace a key-value pair into the map.
             *
             * This method constructs a Pair<Key, Value> in place using
             * perfect forwarding of the provided key and value arguments,
             * and then inserts it into the underlying Red-Black Tree.
             *
             * @param key The key to insert.
             * @param value The value associated with the key.
             * @return NodeType Pointer to the newly inserted node.
             */
            template<
                class U = Key,
                class V = Value,
                typename = Trait::EnsureSame<U, Key>,
                typename = Trait::EnsureSame<V, Value>
            >
            NodeType EmplaceBase(U &&key, V &&value)
            {
                // Create a Pair in place
                PairType pair(std::forward<U>(key), std::forward<V>(value));

                // Insert the pair into the Red-Black Tree
                return this->Insert(pair);
            }

            /*
             * @brief Helper method to locate a node by key without copying.
             *
             * This method traverses the Red-Black Tree to find the node
             * containing the specified key. It uses arithmetic comparisons
             * to navigate the tree.
             *
             * @param key The key to locate.
             * @return NodeType Pointer to the located node, or nullptr if not found.
             */
            template<
                class U = Key,
                typename = Trait::EnsureSame<U, Key>
            >
            NodeType LocateNoCopy(U &&key)
            {
                NodeType current = this->root;
                while (current != nullptr)
                {
                    if (ArithCompare::Lt(std::forward<U>(key), current->data.First()))
                    {
                        current = current->left;
                    }
                    else if (ArithCompare::Gt(std::forward<U>(key), current->data.First()))
                    {
                        current = current->right;
                    }
                    else
                    {
                        return current; // Found
                    }
                }

                return nullptr; // Not found
            }
        public:
            // Inherit constructors from RedBlack base class
            using Tree::Pmr::RedBlack<
                Misc::Pair<Key, Value>,
                EqCompare,
                ArithCompare,
                CleanupGrowthFactor,
                CleanupGrowthInitialCapacity,
                Allocator
            >::RedBlack;

            /*
             * @brief Inserts a key-value pair into the map by emplacing them.
             *
             * This method constructs a Pair<Key, Value> in place using
             * perfect forwarding of the provided key and value arguments,
             * and then inserts it into the underlying Red-Black Tree.
             *
             * @param key The key to insert.
             * @param value The value associated with the key.
             */
            template<
                class U = Key,
                class V = Value,
                typename = Trait::EnsureSame<U, Key>,
                typename = Trait::EnsureSame<V, Value>
            >
            void Emplace(U &&key, V &&value)
            {
                // Delegate to EmplaceBase
                EmplaceBase(std::forward<U>(key), std::forward<V>(value));
            }

            /*
             * @brief Access or insert the value associated with the given key.
             *
             * If the key exists in the map, returns a reference to the associated value.
             * If the key does not exist, a new entry is created with a default-constructed
             * Value, and a reference to this new value is returned.
             *
             * @param key The key to access or insert.
             * @return Reference to the value associated with the key.
             */
            Value &At(const Key &key)
            {
                // Try to locate the key in the tree
                auto node = LocateNoCopy(key);

                // If not found, create a new entry with default Value
                if (node == nullptr)
                {
                    // Emplace a new key-value pair with default-constructed Value
                    node = EmplaceBase(key, Value{});

                    // Return reference to the newly created value
                    return node->data.Second();
                }

                // Return reference to the found value
                return node->data.Second();
            }

            /*
             * @brief Subscript operator to access or insert values by key.
             *
             * This operator provides convenient access to values in the map
             * using the subscript syntax. It forwards to the At() method.
             *
             * @param key The key to access or insert.
             * @return Reference to the value associated with the key.
             */
            template<
                class U = Key,
                typename = Trait::EnsureSame<U, Key>
            >
            Value &operator[](U &&key)
            {
                return At(std::forward<U>(key));
            }

            /*
             * @brief Check if the map contains a key.
             *
             * This method checks if the specified key exists in the map
             * by attempting to locate it in the underlying Red-Black Tree.
             *
             * @param key The key to check for containment.
             * @return true if the key exists in the map, false otherwise.
             */
            template<
                class U = Key,
                typename = Trait::EnsureSame<U, Key>
            >
            void Contains(U &&key)
            {
                return LocateNoCopy(std::forward<U>(key)) != nullptr;
            }

            /*
             * @brief Remove the entry with the specified key from the map.
             *
             * This method locates the node containing the specified key
             * and removes it from the underlying Red-Black Tree if found.
             *
             * @param key The key of the entry to remove.
             */
            template<
                class U = Key,
                typename = Trait::EnsureSame<U, Key>
            >
            void Remove(U &&key)
            {
                auto node = LocateNoCopy(std::forward<U>(key));
                if (node != nullptr)
                {
                    this->RemoveBase(node);
                }
            }

            /*
             * @brief Subtraction assignment operator to remove a key from the map.
             *
             * This operator provides a convenient syntax to remove an entry
             * from the map using the `-=` operator with the specified key.
             *
             * @param key The key of the entry to remove.
             * @return Reference to the modified map.
             */
            template<
                class U = Key,
                typename = Trait::EnsureSame<U, Key>
            >
            Map &operator-=(U &&key)
            {
                Remove(std::forward<U>(key));
                return *this;
            }
        };
    }

    /*
     * @brief PMR-enabled Map container alias.
     *
     * This alias template provides a convenient way to define a Map
     * using the default PMR allocator. It is equivalent to
     * Celery::Collection::Pmr::Map with default allocator.
     *
     * @tparam Key Type of the keys in the map.
     * @tparam Value Type of the values in the map.
     */
    template<typename Key, typename Value>
    using Map = Pmr::Map<Key, Value>;
}