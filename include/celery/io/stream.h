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
#include "celery/array/ring.h"
#include "celery/io/std.h"
#include "celery/trait/default.h"
#include "celery/string/string.h"
#include "celery/except/bad_read.h"

namespace Celery::Io
{
    namespace Pmr
    {
        /**
         * @brief Output stream implementation using Celery's ring buffer.
         *
         * This class provides a high-performance buffered output stream
         * that writes data to a user-specified descriptor (e.g. stdout or stderr)
         * using Celery’s PMR (Polymorphic Memory Resource) allocators.
         *
         * @tparam Desc        Output descriptor type (must derive from Descriptor)
         * @tparam Capacity    Buffer capacity (in bytes)
         * @tparam UseHeap     Whether to allocate the buffer on the heap (defaults true if Capacity >= 1024)
         * @tparam Allocator   Allocator type (defaults to Celery::Pmr::ArrayAllocator<char>)
         * @tparam             (SFINAE) Ensures Desc derives from Descriptor
         */
        template<
            typename Desc = Stdout,
            unsigned int Capacity = 2048,
            bool UseHeap = Capacity >= 1024,
            typename Allocator = Celery::Pmr::ArrayAllocator<char>,
            typename = std::enable_if_t<
                std::is_base_of_v<
                    WriteDescriptor,
                    Desc
                >
            >
        >
        class OStream :
            public Buffer::Pmr::Ring<char, Capacity, UseHeap, Allocator>
        {
            /**
             * @brief Writes a block of data directly to the descriptor.
             *
             * @param data  Pointer to the data to be written.
             * @param count Number of bytes to write.
             */
            void WriteStream(char *data, size_t count)
            {
                Desc::Write(data, count);
            }

        protected:
            /**
             * @brief Handles a batch of buffered data when full or flushed.
             *
             * This overrides the base ring buffer behavior to write
             * data through the descriptor when the buffer is ready.
             *
             * @param data  Pointer to the buffered data.
             * @param count Number of bytes to write.
             */
            void HandleBatch(char *data, size_t count) override
            {
                this->WriteStream(data, count);
            }

        public:
            /** @brief Default constructor. */
            OStream() = default;

            /**
             * @brief Copy constructor.
             *
             * Copies the buffered data from another OStream instance
             * and writes it to this instance's descriptor.
             *
             * @param other The other OStream to copy from.
             */
            OStream(const OStream &other)
            {
                this->Batch(other.Ptr(), other.Len());
            }

            /**
             * @brief Move constructor.
             *
             * Moves buffered data from another OStream and clears the source.
             *
             * @param other The OStream to move from.
             */
            OStream(OStream &&other) noexcept
            {
                this->Batch(other.Ptr(), other.Len());
                other.len = 0;
            }

            /**
             * @brief Flushes the buffer to the output descriptor.
             *
             * Writes all buffered data and clears the buffer.
             */
            void Flush()
            {
                this->WriteStream(this->Ptr(), this->Len());
                this->Clear();
            }

            /**
             * @brief Destructor.
             *
             * Ensures that any remaining buffered data is written
             * before the stream is destroyed.
             */
            ~OStream() override
            {
                this->WriteStream(this->Ptr(), this->Len());
            }
        };

        template<
            typename Desc = Stdin,
            Trait::Decimal GrowthFactor = Trait::GrowthFactor,
            Trait::Uint InitialCapacity = Trait::InitialCapacity,
            Trait::Uint InlineBufferSize = 256,
            typename Allocator = Celery::Pmr::ArrayAllocator<char>,
            typename = std::enable_if_t<
                std::is_base_of_v<
                    ReadDescriptor,
                    Desc
                >
            >
        >
        class IStream
        {
            using Result = Celery::Str::Pmr::String<
                GrowthFactor,
                InitialCapacity,
                Allocator
            >;

        public:
            Result Read()
            {
                Result result;
                char buffer[InlineBufferSize];
                Trait::SignedVeryLarge bytes_read = 0;

                // Read in chunks until no more data is available
                while(true)
                {
                    bytes_read = Desc::Read(buffer, sizeof(buffer));
                    if (bytes_read > 0)
                    {
                        // Scan for newline character
                        for (Trait::VeryLarge i = 0; i < static_cast<Trait::VeryLarge>(bytes_read); ++i)
                        {
                            if (buffer[i] == '\n')
                            {
                                result.Append(buffer, i); // Append up to newline
                                return result; // Stop reading at newline
                            }
                        }

                        result.Append(buffer, bytes_read); // Append read data
                    }
                    else if (bytes_read == 0)
                    {
                        break; // End of input
                    }
                    else
                    {
                        throw Except::BadRead(); // Read error
                    }
                }

                return result;
            }
        };
    }

    /**
     * @brief Alias for Celery::Io::Pmr::OStream with default allocator settings.
     *
     * @tparam Desc      Output descriptor (default: Stdout)
     * @tparam Capacity  Buffer capacity (default: 2048)
     */
    template<typename Desc = Stdout, unsigned int Capacity = 2048>
    using OStream = Pmr::OStream<Desc, Capacity>;

    /**
     * @brief Alias for Celery::Io::Pmr::IStream with default allocator settings.
     *
     * @tparam Desc      Input descriptor (default: Stdin)
     */
    template<typename Desc = Stdin>
    using IStream = Pmr::IStream<Desc>;

    /**
     * @brief Thread-local default output streams.
     *
     * @deprecated Use Celery::Io::Print instead.
     */
    inline thread_local OStream<> IStdout;
    inline thread_local OStream<Stderr> IStderr;
    inline IStream<> IStdin;
}