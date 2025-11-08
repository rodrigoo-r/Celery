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
        bool transferred; // Indicates if resources have been transferred

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
        template<
            class U = Fst,
            class V = Snd,
            typename = Trait::EnsureSame<U, Fst>,
            typename = Trait::EnsureSame<V, Snd>
        >
        Pair(U &&first, V &&second)
        {
            // Placement new to construct the elements in the aligned storage
            new (fst) Fst(first);
            new (snd) Snd(second);
        }

        /**
         * @brief Copy constructor for Pair.
         *
         * This constructor copies the elements from another Pair instance
         * into this one using placement new.
         *
         * @param other The Pair instance to copy from.
         */
        Pair(const Pair &other)
        {
            // Placement new to copy-construct the elements
            new (fst) Fst(other.fst);
            new (snd) Snd(other.snd);
        }

        /**
         * @brief Move constructor for Pair.
         *
         * This constructor moves the elements from another Pair instance
         * into this one using placement new.
         *
         * @param other The Pair instance to move from.
         */
        Pair(Pair &&other) noexcept
        {
            // Placement new to move-construct the elements
            new (fst) Fst(std::move(other.First()));
            new (snd) Snd(std::move(other.Second()));

            // Mark the other as transferred
            other.transferred = true;
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
            if (transferred) return; // Skip if resources were transferred
            reinterpret_cast<Fst *>(fst)->~Fst();
            reinterpret_cast<Snd *>(snd)->~Snd();
        }
    };

    /**
     * @brief Get function to access elements of the Pair by index.
     *
     * This function template allows accessing the first or second element
     * of the Pair using an index (0 for first, 1 for second).
     *
     * @tparam I Index of the element to access (0 or 1).
     * @tparam Fst Type of the first element.
     * @tparam Snd Type of the second element.
     * @param p The Pair instance to access.
     * @return Reference to the requested element.
     */
    template <Trait::VeryLarge I, typename Fst, typename Snd>
    decltype(auto) get(Pair<Fst, Snd>& p)
    {
        if constexpr (I == 0)
            return p.First();
        else
            return p.Second();
    }

    // Const overload
    template <Trait::VeryLarge I, typename Fst, typename Snd>
    decltype(auto) get(const Pair<Fst, Snd>& p)
    {
        if constexpr (I == 0)
            return p.First();
        else
            return p.Second();
    }

    // Rvalue overload
    template <Trait::VeryLarge I, typename Fst, typename Snd>
    decltype(auto) get(Pair<Fst, Snd>&& p)
    {
        if constexpr (I == 0)
            return std::move(p.First());
        else
            return std::move(p.Second());
    }
}

// Specialize std::tuple_size and std::tuple_element for Celery::Misc::Pair
// to enable structured bindings and tuple-like access.
namespace std
{
    /**
     * @brief Specialization of std::tuple_size for Celery::Misc::Pair.
     *
     * This specialization defines the size of the Pair as 2.
     *
     * @tparam Fst Type of the first element.
     * @tparam Snd Type of the second element.
     */
    template <typename Fst, typename Snd>
    struct tuple_size<Celery::Misc::Pair<Fst, Snd>> : std::integral_constant<std::size_t, 2>
        {};

    /**
     * @brief Specialization of std::tuple_element for Celery::Misc::Pair.
     *
     * This specialization provides access to the types of the elements
     * in the Pair by index (0 for first, 1 for second).
     *
     * @tparam I Index of the element (0 or 1).
     * @tparam Fst Type of the first element.
     * @tparam Snd Type of the second element.
     */
    template <typename Fst, typename Snd>
    struct tuple_element<0, Celery::Misc::Pair<Fst, Snd>>
    {
        using type = Fst;
    };

    /**
     * @brief Specialization of std::tuple_element for Celery::Misc::Pair.
     *
     * This specialization provides access to the types of the elements
     * in the Pair by index (0 for first, 1 for second).
     *
     * @tparam I Index of the element (0 or 1).
     * @tparam Fst Type of the first element.
     * @tparam Snd Type of the second element.
     */
    template <typename Fst, typename Snd>
    struct tuple_element<1, Celery::Misc::Pair<Fst, Snd>>
    {
        using type = Snd;
    };
}