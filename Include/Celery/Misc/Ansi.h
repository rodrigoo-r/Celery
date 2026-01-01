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

#include "Celery/Util/Macro.h"
#define CELERY_ANSI_CODE_RESET          0
#define CELERY_ANSI_CODE_BOLD           1
#define CELERY_ANSI_CODE_DIM            2
#define CELERY_ANSI_CODE_ITALIC         3
#define CELERY_ANSI_CODE_UNDERLINE      4

#define CELERY_ANSI_CODE_REGULAR_BLACK   30
#define CELERY_ANSI_CODE_REGULAR_RED     31
#define CELERY_ANSI_CODE_REGULAR_GREEN   32
#define CELERY_ANSI_CODE_REGULAR_YELLOW  33
#define CELERY_ANSI_CODE_REGULAR_BLUE    34
#define CELERY_ANSI_CODE_REGULAR_MAGENTA 35
#define CELERY_ANSI_CODE_REGULAR_CYAN    36
#define CELERY_ANSI_CODE_REGULAR_WHITE   37

#define CELERY_ANSI_CODE_BRIGHT_BLACK    90
#define CELERY_ANSI_CODE_BRIGHT_RED      91
#define CELERY_ANSI_CODE_BRIGHT_GREEN    92
#define CELERY_ANSI_CODE_BRIGHT_YELLOW   93
#define CELERY_ANSI_CODE_BRIGHT_BLUE     94
#define CELERY_ANSI_CODE_BRIGHT_MAGENTA  95
#define CELERY_ANSI_CODE_BRIGHT_CYAN     96
#define CELERY_ANSI_CODE_BRIGHT_WHITE    97

#define CELERY_ANSI_FROM_RGB(R, G, B)   \
    "38;2;" CELERY_MACRO_STR(R) ";"     \
    CELERY_MACRO_STR(G) ";"             \
    CELERY_MACRO_STR(B)

#define CELERY_ANSI_EXPAND_COLOR(PREFIX, NAME) \
    CELERY_MACRO_STR(CELERY_ANSI_CODE_##PREFIX##_##NAME)

#define CELERY_ANSI_EXPAND_COLOR_N_PREFIX(NAME) \
    CELERY_MACRO_STR(CELERY_ANSI_CODE_##NAME) ";"

#define CELERY_ANSI_GENERATE_CODE(...) \
    "\033[" \
    __VA_ARGS__ \
    "m"

#define CELERY_ANSI_GENERATE_VARIABLES(PREFIX, ...)     \
    inline constexpr char Reset[] =                     \
        CELERY_ANSI_GENERATE_CODE(                      \
            __VA_ARGS__                                 \
            CELERY_MACRO_STR(CELERY_ANSI_CODE_RESET)    \
        );                                              \
                                                        \
    inline constexpr char Black[] =                     \
        CELERY_ANSI_GENERATE_CODE(                      \
            __VA_ARGS__                                 \
            CELERY_ANSI_EXPAND_COLOR(PREFIX, BLACK)     \
        );                                              \
                                                        \
    inline constexpr char Red[] =                       \
        CELERY_ANSI_GENERATE_CODE(                      \
            __VA_ARGS__                                 \
            CELERY_ANSI_EXPAND_COLOR(PREFIX, RED)       \
        );                                              \
                                                        \
    inline constexpr char Green[] =                     \
        CELERY_ANSI_GENERATE_CODE(                      \
            __VA_ARGS__                                 \
            CELERY_ANSI_EXPAND_COLOR(PREFIX, GREEN)     \
        );                                              \
                                                        \
    inline constexpr char Yellow[] =                    \
        CELERY_ANSI_GENERATE_CODE(                      \
            __VA_ARGS__                                 \
            CELERY_ANSI_EXPAND_COLOR(PREFIX, YELLOW)    \
        );                                              \
                                                        \
    inline constexpr char Blue[] =                      \
        CELERY_ANSI_GENERATE_CODE(                      \
            __VA_ARGS__                                 \
            CELERY_ANSI_EXPAND_COLOR(PREFIX, BLUE)      \
        );                                              \
                                                        \
    inline constexpr char Magenta[] =                   \
        CELERY_ANSI_GENERATE_CODE(                      \
            __VA_ARGS__                                 \
            CELERY_ANSI_EXPAND_COLOR(PREFIX, MAGENTA)   \
        );                                              \
                                                        \
    inline constexpr char Cyan[] =                      \
        CELERY_ANSI_GENERATE_CODE(                      \
            __VA_ARGS__                                 \
            CELERY_ANSI_EXPAND_COLOR(PREFIX, CYAN)      \
        );                                              \
                                                        \
    inline constexpr char White[] =                     \
        CELERY_ANSI_GENERATE_CODE(                      \
            __VA_ARGS__                                 \
            CELERY_ANSI_EXPAND_COLOR(PREFIX, WHITE)     \
        );                                              \

namespace Celery::Misc::Ansi
{
    CELERY_ANSI_GENERATE_VARIABLES(REGULAR)

    // Dim ANSI codes
    namespace Dim
    {
        // Dim ANSI codes
        CELERY_ANSI_GENERATE_VARIABLES(
            REGULAR,
            CELERY_ANSI_EXPAND_COLOR_N_PREFIX(DIM)
        )

        namespace Italic
        {
            // Dim Italic ANSI codes
            CELERY_ANSI_GENERATE_VARIABLES(
                REGULAR,
                CELERY_ANSI_EXPAND_COLOR_N_PREFIX(DIM)
                CELERY_ANSI_EXPAND_COLOR_N_PREFIX(ITALIC)
            )

            namespace Underline
            {
                // Dim Italic Underline ANSI codes
                CELERY_ANSI_GENERATE_VARIABLES(
                    REGULAR,
                    CELERY_ANSI_EXPAND_COLOR_N_PREFIX(DIM)
                    CELERY_ANSI_EXPAND_COLOR_N_PREFIX(ITALIC)
                    CELERY_ANSI_EXPAND_COLOR_N_PREFIX(UNDERLINE)
                )
            }
        }

        namespace Underline
        {
            // Dim Underline ANSI codes
            CELERY_ANSI_GENERATE_VARIABLES(
                REGULAR,
                CELERY_ANSI_EXPAND_COLOR_N_PREFIX(DIM)
                CELERY_ANSI_EXPAND_COLOR_N_PREFIX(UNDERLINE)
            )

            namespace Italic
            {
                // Dim Underline Italic ANSI codes
                CELERY_ANSI_GENERATE_VARIABLES(
                    REGULAR,
                    CELERY_ANSI_EXPAND_COLOR_N_PREFIX(DIM)
                    CELERY_ANSI_EXPAND_COLOR_N_PREFIX(UNDERLINE)
                    CELERY_ANSI_EXPAND_COLOR_N_PREFIX(ITALIC)
                )
            }
        }
    }

    namespace Italic
    {
        // Italic ANSI codes
        CELERY_ANSI_GENERATE_VARIABLES(
            REGULAR,
            CELERY_ANSI_EXPAND_COLOR_N_PREFIX(ITALIC)
        )
    }

    namespace Underline
    {
        // Underline ANSI codes
        CELERY_ANSI_GENERATE_VARIABLES(
            REGULAR,
            CELERY_ANSI_EXPAND_COLOR_N_PREFIX(UNDERLINE)
        )
    }

    namespace Bright
    {
        // Bright ANSI codes
        CELERY_ANSI_GENERATE_VARIABLES(BRIGHT)

        // Dim Bright ANSI codes
        namespace Dim
        {
            CELERY_ANSI_GENERATE_VARIABLES(
                BRIGHT,
                CELERY_ANSI_EXPAND_COLOR_N_PREFIX(DIM)
            )
        }

        namespace Italic
        {
            // Bright Italic ANSI codes
            CELERY_ANSI_GENERATE_VARIABLES(
                BRIGHT,
                CELERY_ANSI_EXPAND_COLOR_N_PREFIX(ITALIC)
            )
        }

        namespace Underline
        {
            // Bright Underline ANSI codes
            CELERY_ANSI_GENERATE_VARIABLES(
                BRIGHT,
                CELERY_ANSI_EXPAND_COLOR_N_PREFIX(UNDERLINE)
            )
        }
    }

    namespace Bold
    {
        // Bold ANSI codes
        CELERY_ANSI_GENERATE_VARIABLES(
            REGULAR,
            CELERY_ANSI_EXPAND_COLOR_N_PREFIX(BOLD)
        )

        namespace Bright
        {
            // Bright Bold ANSI codes
            CELERY_ANSI_GENERATE_VARIABLES(
                BRIGHT,
                CELERY_ANSI_EXPAND_COLOR_N_PREFIX(BOLD)
            )
        }

        namespace Dim
        {
            // Bold Dim ANSI codes
            CELERY_ANSI_GENERATE_VARIABLES(
                REGULAR,
                CELERY_ANSI_EXPAND_COLOR_N_PREFIX(BOLD)
                CELERY_ANSI_EXPAND_COLOR_N_PREFIX(DIM)
            )

            namespace Bright
            {
                // Bright Bold ANSI codes
                CELERY_ANSI_GENERATE_VARIABLES(
                    BRIGHT,
                    CELERY_ANSI_EXPAND_COLOR_N_PREFIX(BOLD)
                    CELERY_ANSI_EXPAND_COLOR_N_PREFIX(DIM)
                )
            }
        }

        namespace Italic
        {
            // Bold Italic ANSI codes
            CELERY_ANSI_GENERATE_VARIABLES(
                REGULAR,
                CELERY_ANSI_EXPAND_COLOR_N_PREFIX(BOLD)
                CELERY_ANSI_EXPAND_COLOR_N_PREFIX(ITALIC)
            )

            namespace Bright
            {
                // Bright Bold Italic ANSI codes
                CELERY_ANSI_GENERATE_VARIABLES(
                    BRIGHT,
                    CELERY_ANSI_EXPAND_COLOR_N_PREFIX(BOLD)
                    CELERY_ANSI_EXPAND_COLOR_N_PREFIX(ITALIC)
                )
            }
        }

        namespace Underline
        {
            // Bold Underline ANSI codes
            CELERY_ANSI_GENERATE_VARIABLES(
                REGULAR,
                CELERY_ANSI_EXPAND_COLOR_N_PREFIX(BOLD)
                CELERY_ANSI_EXPAND_COLOR_N_PREFIX(UNDERLINE)
            )

            namespace Bright
            {
                // Bright Bold Underline ANSI codes
                CELERY_ANSI_GENERATE_VARIABLES(
                    BRIGHT,
                    CELERY_ANSI_EXPAND_COLOR_N_PREFIX(BOLD)
                    CELERY_ANSI_EXPAND_COLOR_N_PREFIX(UNDERLINE)
                )
            }
        }
    }
}