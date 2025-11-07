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

namespace Celery::Misc
{
    /**
     * @brief A simple pair container holding two elements of potentially different types.
     *
     * This class provides storage for two elements, `First` and `Second`, of types
     * `Fst` and `Snd` respectively. It uses placement new to construct the elements
     * in aligned storage and provides accessors to retrieve references to them.
     *
     * @tparam Fst Type of the first element.
     * @tparam Snd Type of the second element.
     */
    template <typename Fst, typename Snd>
    class Pair
    {
    protected:
        // Storage for the two elements
        alignas(Fst) unsigned char fst[sizeof(Fst)];
        alignas(Snd) unsigned char snd[sizeof(Snd)];

    public:
        /**
         * @brief Constructs a Pair with the given first and second elements.
         *
         * This constructor uses placement new to construct the `Fst` and `Snd`
         * elements in the pre-allocated aligned storage.
         *
         * @param first The value to initialize the first element.
         * @param second The value to initialize the second element.
         */
        Pair(const Fst &first, const Snd &second)
        {
            // Placement new to construct the elements in the aligned storage
            new (fst) Fst(first);
            new (snd) Snd(second);
        }

        /**
         * @brief Accessor for the first element.
         *
         * @return Reference to the first element of type `Fst`.
         */
        Fst &First()
        {
            return *reinterpret_cast<Fst *>(fst);
        }

        /**
         * @brief Accessor for the second element.
         *
         * @return Reference to the second element of type `Snd`.
         */
        Snd &Second()
        {
            return *reinterpret_cast<Snd *>(snd);
        }

        /**
         * @brief Destructor for the Pair.
         *
         * Calls the destructors of the contained elements.
         */
        ~Pair()
        {
            reinterpret_cast<Fst *>(fst)->~Fst();
            reinterpret_cast<Snd *>(snd)->~Snd();
        }
    };
}
