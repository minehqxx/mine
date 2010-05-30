/* ===-- endianness.h - configuration header for compiler-rt ---------------===
 *
 *		       The LLVM Compiler Infrastructure
 *
 * This file is distributed under the University of Illinois Open Source
 * License.
 *
 * ==============================================================================
 * LLVM Release License
 * ==============================================================================
 * University of Illinois/NCSA
 * Open Source License
 *
 * Copyright (c) 2003-2009 University of Illinois at Urbana-Champaign.
 * All rights reserved.
 *
 * Developed by:
 *
 *     LLVM Team
 *
 *     University of Illinois at Urbana-Champaign
 *
 *     http://llvm.org
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal with
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 *
 *     * Redistributions of source code must retain the above copyright notice,
 *       this list of conditions and the following disclaimers.
 *
 *     * Redistributions in binary form must reproduce the above copyright notice,
 *       this list of conditions and the following disclaimers in the
 *       documentation and/or other materials provided with the distribution.
 *
 *     * Neither the names of the LLVM Team, University of Illinois at
 *       Urbana-Champaign, nor the names of its contributors may be used to
 *       endorse or promote products derived from this Software without specific
 *       prior written permission.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS WITH THE
 * SOFTWARE.
 *
 *
 * ===----------------------------------------------------------------------===
 *
 * This file is a configuration header for compiler-rt.
 * This file is not part of the interface of this library.
 *
 * ===----------------------------------------------------------------------===
 */

#ifndef ENDIANNESS_H
#define ENDIANNESS_H

/*
 * Known limitations:
 *   Middle endian systems are not handled currently.
 */

#if defined(__SVR4) && defined(__sun)
#include <sys/byteorder.h>

#if _BYTE_ORDER == _BIG_ENDIAN
#define _YUGA_LITTLE_ENDIAN 0
#define _YUGA_BIG_ENDIAN    1
#elif _BYTE_ORDER == _LITTLE_ENDIAN 
#define _YUGA_LITTLE_ENDIAN 1
#define _YUGA_BIG_ENDIAN    0
#endif /* _BYTE_ORDER */

#endif /* Solaris and AuroraUX. */

/* .. */

#if defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__DragonFly__)
#include <sys/endian.h>

#if _BYTE_ORDER == _BIG_ENDIAN
#define _YUGA_LITTLE_ENDIAN 0
#define _YUGA_BIG_ENDIAN    1
#elif _BYTE_ORDER == _LITTLE_ENDIAN
#define _YUGA_LITTLE_ENDIAN 1
#define _YUGA_BIG_ENDIAN    0
#endif /* _BYTE_ORDER */

#endif /* *BSD */

/* .. */

/* Mac OSX has __BIG_ENDIAN__ or __LITTLE_ENDIAN__ automatically set by the compiler (at least with GCC) */
#if defined(__APPLE__) && defined(__MACH__) || defined(__ellcc__ )

#ifdef __BIG_ENDIAN__
#if __BIG_ENDIAN__
#define _YUGA_LITTLE_ENDIAN 0
#define _YUGA_BIG_ENDIAN    1
#endif
#endif /* __BIG_ENDIAN__ */

#ifdef __LITTLE_ENDIAN__
#if __LITTLE_ENDIAN__
#define _YUGA_LITTLE_ENDIAN 1
#define _YUGA_BIG_ENDIAN    0
#endif
#endif /* __LITTLE_ENDIAN__ */

#endif /* Mac OSX */

/* .. */

#if defined(__linux__)
#include <endian.h>

#if __BYTE_ORDER == __BIG_ENDIAN
#define _YUGA_LITTLE_ENDIAN 0
#define _YUGA_BIG_ENDIAN    1
#elif __BYTE_ORDER == __LITTLE_ENDIAN
#define _YUGA_LITTLE_ENDIAN 1
#define _YUGA_BIG_ENDIAN    0
#endif /* __BYTE_ORDER */

#endif /* GNU/Linux */

/* . */

#if !defined(_YUGA_LITTLE_ENDIAN) || !defined(_YUGA_BIG_ENDIAN)
#error Unable to determine endian
#endif /* Check we found an endianness correctly. */

#endif /* ENDIANNESS_H */
