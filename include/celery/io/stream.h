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
#include "../array/ring.h"
#include "std.h"

namespace Celery::Io
{
    namespace Pmr
    {
        template<
            typename Desc = Stdout,
            unsigned int Capacity = 1024,
            bool UseHeap = Capacity >= 1024,
            typename Allocator = Celery::Pmr::ArrayAllocator<char>,
            // SFINAE to check that the Descriptor is valid
            typename = std::enable_if_t<
                std::is_base_of_v<
                    Descriptor,
                    Desc
                >
            >
        >
        class OStream : public Buffer::Pmr::Ring<char, Capacity, UseHeap, Allocator>
        {
            void WriteStream(char *data, size_t count)
            {
                Desc::Write(data, count);
            }

        protected:
            void HandleBatch(char *data, size_t count) override
            {
                // Write the batch to the descriptor
                this->WriteStream(data, count);
            }

        public:
            void Flush()
            {
                this->WriteStream(this->Ptr(), this->Len());
                this->Clear();
            }

            ~OStream() override
            {
                // Write buffered data to the descriptor
                this->WriteStream(this->Ptr(), this->Len());
            }
        };
    }

    template<typename Desc = Stdout, unsigned int Capacity = 2048>
    using OStream = Pmr::OStream<Desc, Capacity>;

    // Default output stream
    // @deprecated, use Celery::Io::Print instead
    inline OStream<> IStdout;
    inline OStream<Stderr> IStderr;
}