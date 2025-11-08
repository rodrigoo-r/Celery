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
#include "celery/tree/rb.h"

namespace Celery::Collection
{
    /**
     * @namespace Celery::Collection::Pmr
     * @brief Namespace containing polymorphic-memory-resource (PMR) aware collection types.
     *
     * Collections in this namespace are designed to work with PMR-style allocators provided
     * by Celery::Pmr. They offer STL-like interfaces while allowing efficient memory
     * management policies (for example, monotonic allocation) for high-performance use.
     */
    namespace Pmr
    {
        /**
         * @brief Alias for a Red-Black tree based set using PMR-aware allocator.
         *
         * This template defines a Set type implemented as a Red-Black tree under
         * `Tree::Pmr::RedBlack<Key, Allocator>`. By default it uses a monotonic
         * allocator specialized for `Tree::Pmr::RedBlackNode<Key>`, which is suitable
         * for workloads that allocate many nodes and can free them all at once or
         * at predictable times.
         *
         * @tparam Key The key type stored in the set. Keys are compared using the
         *             Red-Black tree's comparison semantics.
         * @tparam Allocator The allocator type to use for node allocation. Defaults to
         *                   `Celery::Pmr::MonotonicAllocator<Tree::Pmr::RedBlackNode<Key>>`.
         *
         * @note The default allocator favors allocation speed and low fragmentation;
         *       choose a different allocator if you need per-node deallocation or
         *       different memory lifetime semantics.
         *
         * @see Tree::Pmr::RedBlack
         * @see Celery::Pmr::MonotonicAllocator
         */
        template<
            typename Key,
            typename Allocator = Celery::Pmr::MonotonicAllocator<
                Tree::Pmr::RedBlackNode<Key>
            >,
            typename EqCompare = Base::EqualityCompare<Key>,
            typename ArithCompare = Base::ArithmeticCompare<Key>
        >
        using Set = Tree::Pmr::RedBlack<Key, Allocator, EqCompare, ArithCompare>;
    }

    /**
     * @brief Convenience alias for PMR Set using the default PMR allocator.
     *
     * This alias exposes the PMR-aware `Set` at the `Celery::Collection` level so
     * users can write `Celery::Collection::Set<Key>` and obtain the PMR-backed
     * Red-Black tree with default allocation strategy.
     *
     * @tparam Key The key type stored in the set.
     */
    template<typename Key>
    using Set = Pmr::Set<Key>;
}
