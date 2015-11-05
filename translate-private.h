/**
 * @file translate-private.h
 * @brief Declarations for translate class
 * @author Nicu Tofan <nicu.tofan@gmail.com>
 * @copyright Copyright 2014 piles contributors. All rights reserved.
 * This file is released under the
 * [MIT License](http://opensource.org/licenses/mit-license.html)
 */

#ifndef GUARD_TRANSLATE_PRIVATE_H_INCLUDE
#define GUARD_TRANSLATE_PRIVATE_H_INCLUDE

#include <translate/translate-config.h>

#if 0
#    define TRANSLATE_DEBUGM printf
#else
#    define TRANSLATE_DEBUGM black_hole
#endif

#if 0
#    define TRANSLATE_TRACE_ENTRY printf("TRANSLATE ENTRY %s in %s[%d]\n", __func__, __FILE__, __LINE__)
#else
#    define TRANSLATE_TRACE_ENTRY
#endif

#if 0
#    define TRANSLATE_TRACE_EXIT printf("TRANSLATE EXIT %s in %s[%d]\n", __func__, __FILE__, __LINE__)
#else
#    define TRANSLATE_TRACE_EXIT
#endif


static inline void black_hole (...)
{}

#endif // GUARD_TRANSLATE_PRIVATE_H_INCLUDE
