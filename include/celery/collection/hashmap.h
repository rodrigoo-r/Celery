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
#include "celery/array/vector.h"
#include "celery/base/hashable.h"
#include "celery/memory/monotonic.h"
#include "celery/misc/pair.h"
#include "celery/tree/rb.h"
#include "map.h"

namespace Celery::Collection
{
    namespace Pmr
    {
        /**
         * @brief HashMap - a hash-based associative container.
         *
         * This class provides a hashed mapping from Key to Value using a
         * bucketed approach. Each bucket is a Map (red-black tree) and
         * the outer structure keeps a vector of buckets. Hashing and
         * equality comparisons are customizable via template parameters.
         *
         * The template parameters control growth factors, initial capacities,
         * hashing, equality, and underlying allocators used for buckets and
         * inner nodes.
         *
         * @tparam Key Key type.
         * @tparam Value Mapped value type.
         * @tparam BucketGrowthFactor Growth factor used by the outer buckets vector.
         * @tparam BucketInitialCapacity Initial number of buckets.
         * @tparam BucketCleanupGrowthFactor Growth factor for inner bucket maps.
         * @tparam BucketCleanupInitialCapacity Initial capacity for inner bucket maps.
         * @tparam Hash Hash functor/struct providing static Get(Key) -> Trait::Uint.
         * @tparam Equal Key equality comparator.
         * @tparam InnerEqual Inner map key/value equality comparator.
         * @tparam InnerArith Inner map arithmetic comparator.
         * @tparam KeyCompare Key comparison used by inner maps.
         * @tparam BucketCleanupAllocator Allocator type used for bucket cleanup structures.
         * @tparam InnerAllocator Allocator type for inner map nodes.
         * @tparam Allocator Allocator used for the outer buckets vector.
         */
        template<
            typename Key,
            typename Value,
            Trait::Decimal BucketGrowthFactor = Trait::GrowthFactor,
            Trait::Uint BucketInitialCapacity = Trait::InitialCapacity,
            Trait::Decimal BucketCleanupGrowthFactor = Trait::GrowthFactor,
            Trait::Uint BucketCleanupInitialCapacity = Trait::InitialCapacity,
            typename Hash = Base::Hash<Key>,
            typename Equal = Base::EqualityCompare<Key>,
            typename InnerEqual = MapEqualityCompare<Key, Value>,
            typename InnerArith = MapArithmeticCompare<Key, Value>,
            typename KeyCompare = Base::ArithmeticCompare<Key>,
            typename BucketCleanupAllocator = Celery::Pmr::ArrayAllocator<
                Tree::Pmr::RedBlackNode<Misc::Pair<Key, Value>> *
            >,
            typename InnerAllocator = Celery::Pmr::MonotonicAllocator<
                Tree::Pmr::RedBlackNode<Misc::Pair<Key, Value>>
            >,
            typename Allocator = Celery::Pmr::ArrayAllocator<
                Map<
                    Key,
                    Value,
                    InnerEqual,
                    InnerArith,
                    KeyCompare,
                    BucketCleanupGrowthFactor,
                    BucketCleanupInitialCapacity,
                    InnerAllocator,
                    BucketCleanupAllocator
                >
            >,
            typename = Base::EnsureCompare<Equal>,
            typename = Trait::EnsureAllocator<Allocator>
        >
        class HashMap :
            public Base::Resizable,
            public Base::Sizeable,
            public Base::DirectlyPushable<
                HashMap<
                    Key,
                    Value,
                    BucketGrowthFactor,
                    BucketInitialCapacity,
                    BucketCleanupGrowthFactor,
                    BucketCleanupInitialCapacity,
                    Hash,
                    Equal,
                    BucketCleanupAllocator,
                    InnerAllocator,
                    Allocator
                >,
                Misc::Pair<Key, Value>
            >,
            public Base::Removable<
                HashMap<
                    Key,
                    Value,
                    BucketGrowthFactor,
                    BucketInitialCapacity,
                    BucketCleanupGrowthFactor,
                    BucketCleanupInitialCapacity,
                    Hash,
                    Equal,
                    BucketCleanupAllocator,
                    InnerAllocator,
                    Allocator
                >,
                Key
            >
        {
        protected:
            using BucketType = Map<
                Key,
                Value,
                InnerEqual,
                InnerArith,
                KeyCompare,
                BucketCleanupGrowthFactor,
                BucketCleanupInitialCapacity,
                InnerAllocator,
                BucketCleanupAllocator
            >;

            // Buckets array
            Array::Pmr::Vector<
                BucketType,
                BucketGrowthFactor,
                BucketInitialCapacity,
                Allocator
            > buckets;

            // Current load factor
            Trait::Decimal load_factor = 0.0;
        public:
            /**
             * @brief Default constructor.
             *
             * Initializes the outer buckets vector to the initial bucket capacity
             * and constructs each inner bucket map.
             */
            HashMap()
            {
                // Set initial capacity
                capacity = BucketInitialCapacity;

                // Initialize buckets
                for (Trait::Uint i = 0; i < capacity; ++i)
                {
                    buckets.EmplaceBack();
                }
            }

            /**
             * @brief Insert a key/value pair into the map.
             *
             * The key is hashed using the Hash::Get function and placed into the
             * appropriate bucket. If the bucket's inner map handles duplicates,
             * that behavior is determined by the inner Map implementation.
             *
             * @tparam U Type deduced for key; must be the same as Key.
             * @tparam V Type deduced for value; must be the same as Value.
             * @param key Key to insert (forwarded).
             * @param value Value to insert (forwarded).
             */
            template<
                class U,
                class V,
                typename = Trait::EnsureSame<U, Key>,
                typename = Trait::EnsureSame<V, Value>
            >
            void Insert(U &&key, V &&value)
            {
                // Hash the value
                const Trait::Uint hash = Hash::Get(std::forward<U>(key));
                auto index = hash % capacity;

                // Insert into the appropriate bucket
                buckets[index].Emplace(
                    std::forward<U>(key),
                    std::forward<V>(value)
                );
                ++this->len;
            }

            /**
             * @brief Emplace a pair into the hash map.
             *
             * Forwards the provided pair's first and second elements to Insert.
             *
             * @tparam U Must be Misc::Pair<Key, Value>.
             * @param pair Pair containing key and value (forwarded).
             */
            template <
                class U = Misc::Pair<Key, Value>,
                typename = Trait::EnsureSame<U, Misc::Pair<Key, Value>>
            >
            void Emplace(U &&pair)
            {
                // Forward to Insert
                Insert(
                    std::forward<Key>(pair.First()),
                    std::forward<Value>(pair.Second())
                );
            }

            /**
             * @brief Remove an element by key.
             *
             * The key is hashed and the corresponding bucket is instructed to remove
             * any matching element. Decrements the stored length.
             *
             * @tparam U Type deduced for key; must be the same as Key.
             * @param key Key to remove (forwarded).
             */
            template<
                class U,
                typename = Trait::EnsureSame<U, Key>
            >
            void Remove(U &&key)
            {
                // Hash the key
                const Trait::Uint hash = Hash::Get(key);
                auto index = hash % capacity;

                // Remove from the appropriate bucket
                buckets[index].Remove(std::forward<U>(key));
                --this->len;
            }

            /**
             * @brief Clear the entire hash map.
             *
             * Clears each inner bucket and resets the stored length to zero.
             */
            void Clear()
            {
                // Clear all buckets
                for (Trait::Uint i = 0; i < capacity; ++i)
                {
                    buckets[i].Clear();
                }
                this->len = 0;
            }

            /**
             * @brief Resize the outer bucket count.
             *
             * Rehashing and redistribution of elements should be implemented here.
             * Currently a TODO placeholder; callers expecting a resize operation
             * should be aware this is not yet implemented.
             *
             * @param new_capacity New number of buckets to set.
             */
            void Resize(Trait::VeryLarge new_capacity) override
            {
                // TODO!
            }

            /**
             * @brief Access element by key.
             *
             * Returns a reference to the mapped value for the given key. Hashes
             * the key and delegates to the inner bucket's operator[] which
             * provides lookup or insertion semantics depending on inner Map.
             *
             * @tparam U Type deduced for key; must be the same as Key.
             * @param key Key to lookup (forwarded).
             * @return Reference to the mapped value.
             */
            template<
                class U,
                typename = Trait::EnsureSame<U, Key>
            >
            Value &At(U &&key)
            {
                // Hash the key
                const Trait::Uint hash = Hash::Get(key);
                auto index = hash % capacity;

                // Locate in the appropriate bucket
                return buckets[index].operator[](key);
            }
        };
    }

    /**
     * @brief Convenience alias for default Pmr HashMap instantiation.
     *
     * @tparam Key Key type.
     * @tparam Value Value type.
     */
    template <typename Key, typename Value>
    using HashMap = Pmr::HashMap<Key, Value>;
}
