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
         * @tparam MaxLoadFactor Maximum load factor before resizing.
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
            Trait::Decimal MaxLoadFactor = Trait::MaxLoadFactor,
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
            public Base::Sizeable,
            public Base::DirectlyPushable<
                HashMap<
                    Key,
                    Value,
                    MaxLoadFactor,
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
                    MaxLoadFactor,
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

            // Current alive buckets
            Trait::Uint alive_buckets = 0;
        public:
            /**
             * @brief Default constructor.
             *
             * Initializes the outer buckets vector to the initial bucket capacity
             * and constructs each inner bucket map.
             */
            HashMap()
            {
                // Initialize buckets
                for (Trait::Uint i = 0; i < BucketInitialCapacity; ++i)
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
                auto capacity = buckets.Size();

                // Resize if load factor exceeded
                if (this->len >= capacity * MaxLoadFactor)
                {
                    const auto new_capacity = static_cast<Trait::VeryLarge>(
                        capacity * BucketGrowthFactor
                    );

                    Resize(new_capacity);
                }

                // Hash the value
                const Trait::Uint hash = Hash::Get(std::forward<U>(key));
                auto index = hash % capacity;
                auto &bucket = buckets[index];

                // Increment alive buckets if this bucket was empty
                if (bucket.Empty())
                {
                    ++alive_buckets;
                }

                // Insert into the appropriate bucket
                bucket.Emplace(
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
                auto index = hash % buckets.Size();
                auto &bucket = buckets[index];

                // Remove from the appropriate bucket
                bucket.Remove(std::forward<U>(key));

                // Decrement alive buckets if this bucket is now empty
                if (bucket.Empty())
                {
                    --alive_buckets;
                }

                // Decrement length
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
                for (Trait::Uint i = 0; i < buckets.Size(); ++i)
                {
                    buckets[i].Clear();
                }
                this->len = 0;
            }

            /**
             * @brief Resize the hash map to a new capacity.
             *
             * Resizes the outer buckets vector to the specified new capacity.
             * Rehashes all existing elements into the new buckets.
             * Downsizing is not supported; if new_capacity is less than or equal
             * to the current capacity, no action is taken.
             *
             * @param new_capacity New capacity for the hash map.
             */
            void Resize(Trait::VeryLarge new_capacity)
            {
                auto capacity = buckets.Size();
                if (new_capacity <= capacity)
                {
                    return; // No downsizing supported
                }

                // Resize the buckets vector
                buckets.Resize(new_capacity);
                capacity = new_capacity;

                // For each bucket, rehash its elements
                Array::Pmr::Vector<Misc::Pair<Key, Value>> to_rehash;

                // Rehash existing elements into new buckets
                for (auto &el : buckets)
                {
                    // Move elements to temporary storage
                    for (auto &pair : el)
                    {
                        to_rehash.EmplaceBack(
                            Misc::Pair<Key, Value>(
                                std::move(pair.First()),
                                std::move(pair.Second())
                            )
                        );
                    }

                    el.Clear(); // Clear the bucket after moving
                }

                // Clear alive buckets count
                alive_buckets = 0;

                // Create the missing buckets in the vector
                for (auto i = buckets.Size(); i < new_capacity; ++i)
                {
                    buckets.EmplaceBack();
                }

                // Insert back into resized buckets
                for (auto &pair : to_rehash)
                {
                    Insert(
                        std::move(pair.First()),
                        std::move(pair.Second())
                    );
                }
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
                auto index = hash % buckets.Size();

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
