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

#define CELERY_MACRO_STR_HELPER(x) #x
#define CELERY_MACRO_STR(x) CELERY_MACRO_STR_HELPER(x)
#define CELERY_MACRO_CONCAT(a, b) CELERY_MACRO_CONCAT_HELPER(a, b)
#define CELERY_MACRO_CONCAT_HELPER(a, b) a##b