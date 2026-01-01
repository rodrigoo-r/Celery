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

//
// Created by rodrigo on 1/1/26.
//

#pragma once
#include <Celery/String/String.h>
#include <Celery/Except/CouldNotRead.h>

namespace Celery::File
{
    inline Str::String Read(Str::External path)
    {
        FILE *file = fopen(
            path.Ptr(),
            "r"
#           ifdef _WIN32
                "b" // Binary mode on Windows
#           endif
        );
        if (!file)
        {
            throw Except::CouldNotRead(path);
        }

        // Seek to the end to get the size
        fseek(file, 0, SEEK_END);
        const auto size = ftell(file);
        fseek(file, 0, SEEK_SET);
        if (size < 0)
        {
            fclose(file);
            throw Except::CouldNotRead(path);
        }

        // Allocate buffer
        // +1 for null terminator
        char *buffer = Celery::Pmr::ArrayAllocator<char>::Allocate(size + 1);
        auto read_size = fread(buffer, sizeof(char), size, file);
        buffer[size] = '\0';

        // Verify read size
        if (read_size != static_cast<size_t>(size))
        {
            Celery::Pmr::ArrayAllocator<char>::Deallocate(buffer);
            fclose(file);
            throw Except::CouldNotRead(path);
        }

        fclose(file);
        return Celery::Str::String::NoCopy(buffer, size);
    }
}