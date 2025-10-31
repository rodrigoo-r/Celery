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
#include "celery/memory/system.h"
#include "celery/trait/default.h"

namespace Celery::Str
{
    /**
     * @brief PMR-related symbols and containers.
     *
     * This nested namespace contains string implementations and helpers that
     * accept Celery PMR-style allocators (ArrayAllocator-based).
     */
    namespace Pmr
    {
        /**
         * @brief A resizable dynamic array with PMR-style allocator support.
         *
         * This class models a high-performance vector container similar to
         * std::vector but tailored for Celery's allocator and trait system.
         *
         * It stores raw characters (char) and provides convenience string-like
         * operations such as construction from a C-string, obtaining a
         * null-terminated view, copying to a newly allocated buffer, and
         * concatenation/comparison operators.
         *
         * @tparam GrowthFactor Multiplicative growth factor used when expanding capacity.
         * @tparam InitialCapacity Initial number of elements to allocate.
         * @tparam Allocator Allocator type providing Allocate/Deallocate for char.
         *
         * @note Template SFINAE constraints:
         * - \c Allocator must be derived from Celery::Pmr::ArrayAllocator\<char\>.
         *
         * @note Resource ownership:
         * - Instances manage their internal buffer via the base Vector implementation.
         * - \c CStr() returns a pointer to the internal buffer (non-owning). The
         *   returned pointer remains valid until the string is modified or destroyed.
         * - \c CStrCopy() allocates a new buffer using \c Allocator::Allocate and
         *   returns it to the caller; the caller is responsible for freeing it using
         *   the corresponding deallocation routine (e.g. \c Allocator::Deallocate).
         */
        template <
            Trait::Decimal GrowthFactor = Trait::GrowthFactor,
            Trait::Uint InitialCapacity = Trait::InitialCapacity,
            typename Allocator = Celery::Pmr::ArrayAllocator<char>,
            // --- SFINAE Checks --- //
            typename = std::enable_if_t<
                std::is_base_of_v<
                    Celery::Pmr::ArrayAllocator<char>,
                    Allocator
                >
            >
        >
        class String : public Array::Pmr::Vector<char, GrowthFactor, InitialCapacity, Allocator>
        {
        public:
            // Inherit constructors from the base PMR vector.
            using Array::Pmr::Vector<char, GrowthFactor, InitialCapacity, Allocator>::Vector;

            /**
             * @brief Construct a String from a null-terminated C-string.
             *
             * Each character from the C-string is emplaced into the internal
             * buffer. The length is set to the number of characters inserted.
             *
             * @param str Null-terminated C-string to initialize from.
             */
            String(const char *str)
            {
                // Insert each character from the C-string
                for (size_t i = 0; str[i] != '\0'; ++i)
                {
                    this->EmplaceBack(str[i]);
                }
            }

            /**
             * @brief Construct a String from a character buffer and length.
             *
             * Copies \c length bytes from the provided \c str buffer into
             * the internal storage. The length is set to \c length.
             *
             * @param str Pointer to the character buffer.
             * @param length Number of bytes to copy from \c str.
             */
            String(const char *str, size_t length)
            {
                this->Resize(length); // Ensure enough capacity
                memcpy(this->data, str, length);
                this->len = length;
            }

            /**
             * @brief Copy constructor.
             *
             * Creates a new String as a copy of \c other by resizing the internal
             * buffer and copying the bytes.
             *
             * @param other The String to copy from.
             */
            String(String &other)
            {
                this->Resize(other.len);
                memcpy(this->data, other.data, other.len);
                this->len = other.len;
            }

            /**
             * @brief Copy constructor (const version).
             *
             * Creates a new String as a copy of \c other by resizing the internal
             * buffer and copying the bytes.
             *
             * @param other The String to copy from.
             */
            String(const String &other)
            {
                this->Resize(other.len);
                memcpy(this->data, other.data, other.len);
                this->len = other.len;
            }

            /**
             * @brief Move constructor.
             *
             * Transfers ownership of the internal buffer from \c other to this
             * instance, leaving \c other in a valid but empty state.
             *
             * @param other The String to move from.
             */
            String(String &&other) noexcept
            {
                this->data = other.data;
                this->len = other.len;
                this->capacity = other.capacity;

                // Invalidate the other string
                other.data = nullptr;
                other.len = 0;
                other.capacity = 0;
            }

            String() = default;

            /**
             * @brief Return a pointer to a null-terminated view of the string.
             *
             * Ensures there is room for a terminating NUL character, writes the
             * terminator into the internal buffer, and returns the pointer to the
             * internal data.
             *
             * @note The returned pointer is owned by this String instance and must
             * not be freed by the caller. It may be invalidated by non-const
             * operations that modify the string or on destruction.
             *
             * @returns pointer to the internal null-terminated char buffer.
             */
            const char *CStr()
            {
                // Ensure there is enough space for null terminator
                this->EnsureGrowth(1);

                // Add null terminator
                this->data[this->len] = '\0';
                return this->data;
            }

            /**
             * @brief Allocate and return a copy of the string as a null-terminated buffer.
             *
             * Uses the template \c Allocator to allocate a new buffer of size
             * \c len + 1, copies the current string contents, appends a NUL
             * terminator, and returns the allocated pointer.
             *
             * @note The returned buffer is owned by the caller and must be
             * deallocated with the corresponding allocator deallocation method.
             *
             * @returns pointer to newly allocated null-terminated copy.
             */
            const char *CStrCopy()
            {
                // Allocate new buffer for the copy
                char *copy = Allocator::Allocate(this->len + 1);
                // Copy existing data
                std::memcpy(copy, this->data, this->len);
                // Add null terminator
                copy[this->len] = '\0';
                return copy;
            }

            /**
             * @brief Equality comparison with another String.
             *
             * Compares length and the underlying bytes for equality.
             *
             * @param other The other String to compare with.
             * @returns true if lengths and contents are equal; false otherwise.
             */
            bool operator==(const String &other) const
            {
                // Compare lengths and content
                return other.len == this->len &&
                       std::memcmp(other.data, this->data, this->len) == 0;
            }

            /**
             * @brief Equality comparison with a C-string.
             *
             * Computes the length of the C-string and compares bytes.
             *
             * @param other Null-terminated C-string to compare with.
             * @returns true if lengths and contents are equal; false otherwise.
             */
            bool operator==(const char *other) const
            {
                // Get the length of the C-string
                size_t other_len = strlen(other);

                // Compare lengths and content
                return other_len == this->len &&
                       std::memcmp(other, this->data, this->len) == 0;
            }

            /**
             * @brief Append another String to this string (in-place).
             *
             * Ensures enough capacity and appends the bytes from \c other.
             *
             * @param other The String to append.
             * @returns reference to this String after append.
             */
            String &operator+=(const String &other)
            {
                // Ensure enough capacity
                this->EnsureGrowth(this->len + other.len);

                // Copy each character from other
                memcpy(this->data + this->len, other.data, other.len);
                this->len += other.len;
                return *this;
            }

            /**
             * @brief Append a null-terminated C-string to this string (in-place).
             *
             * Computes the length of the C-string, ensures capacity, and copies
             * the characters into the internal buffer.
             *
             * @param other Null-terminated C-string to append.
             * @returns reference to this String after append.
             */
            String &operator+=(const char *other)
            {
                // Get the length of the C-string
                size_t other_len = strlen(other);

                // Ensure enough capacity
                this->EnsureGrowth(this->len + other_len);

                // Copy each character from other
                memcpy(this->data + this->len, other, other_len);
                this->len += other_len;
                return *this;
            }

            /**
             * @brief Append a single character to this string (in-place).
             *
             * Ensures capacity for one more character and appends it.
             *
             * @param other Character to append.
             * @returns reference to this String after append.
             */
            String &operator+=(char other)
            {
                // Ensure enough capacity
                this->EnsureGrowth(this->len + 1);

                // Append the character
                this->data[this->len] = other;
                ++this->len;
                return *this;
            }

            /**
             * @brief Concatenate two Strings and return a new String.
             *
             * Creates a copy of this string and appends \c other to it.
             *
             * @param other String to concatenate.
             * @returns New String containing the concatenation result.
             */
            String operator+(const String &other) const
            {
                // Create a new String as a copy of this
                String result = *this;

                // Append other
                result += other;
                return result;
            }

            /**
             * @brief Concatenate this String with a C-string and return a new String.
             *
             * Creates a copy of this string and appends the null-terminated
             * \c other C-string.
             *
             * @param other Null-terminated C-string to append.
             * @returns New String containing the concatenation result.
             */
            String operator+(const char *other) const
            {
                // Create a new String as a copy of this
                String result = *this;

                // Append other
                result += other;
                return result;
            }

            /**
             * @brief Concatenate this String with a single character and return a new String.
             *
             * Creates a copy of this string and appends \c other.
             *
             * @param other Character to append.
             * @returns New String containing the concatenation result.
             */
            String operator+(char other) const
            {
                // Create a new String as a copy of this
                String result = *this;

                // Append other
                result += other;
                return result;
            }

            /**
             * @brief Copy assignment operator.
             *
             * Deallocates current data, allocates new buffer, and copies
             * contents from \c other.
             *
             * @param other The String to copy from.
             * @returns Reference to this String after assignment.
             */
            String &operator=(const String &other)
            {
                if (this != &other)
                {
                    if (this->data)
                        Allocator::Deallocate(this->data);

                    this->len = other.len;
                    this->capacity = other.capacity;
                    this->data = Allocator::Allocate(this->capacity);
                    std::memcpy(this->data, other.data, this->len);
                }
                return *this;
            }

            /**
             * @brief Move assignment operator.
             *
             * Deallocates current data, transfers ownership from \c other,
             * and leaves \c other in a valid but empty state.
             *
             * @param other The String to move from.
             * @returns Reference to this String after assignment.
             */
            String &operator=(String &&other) noexcept
            {
                if (this != &other)
                {
                    if (this->data)
                        Allocator::Deallocate(this->data);

                    this->data = other.data;
                    this->len = other.len;
                    this->capacity = other.capacity;

                    other.data = nullptr;
                    other.len = 0;
                    other.capacity = 0;
                }
                return *this;
            }

            /**
             * @brief Create a String that references a C-string without copying.
             *
             * Constructs a String instance that points to the provided
             * null-terminated C-string without allocating or copying data.
             * The resulting String's length is set to the length of the
             * C-string (excluding the terminator).
             *
             * @param str Null-terminated C-string to reference.
             * @returns String instance referencing the provided C-string.
             */
            static inline String NoCopy(char *str)
            {
                // Create a String that references the provided C-string without copying
                String result;
                result.len = strlen(str);
                result.data = str;
                result.capacity = result.len;
                return result;
            }
        };
    }

    /// Default alias for the PMR-backed String with default template parameters.
    using String = Pmr::String<>;
}
