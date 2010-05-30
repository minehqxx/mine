/* ===-- int_lib.h - configuration header for compiler-rt  -----------------===
 *
 *                     The LLVM Compiler Infrastructure
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
 * ===----------------------------------------------------------------------===
 *
 * This file is a configuration header for compiler-rt.
 * This file is not part of the interface of this library.
 *
 * ===----------------------------------------------------------------------===
 */

#ifndef INT_LIB_H
#define INT_LIB_H

/* Assumption:  signed integral is 2's complement */
/* Assumption:  right shift of signed negative is arithmetic shift */

#include <limits.h>
#include "endianness.h"
#include <math.h>

/* If compiling for kernel use, call panic() instead of abort(). */
#ifdef KERNEL_USE
extern void panic (const char *, ...);
#define compilerrt_abort() \
  panic("%s:%d: abort in %s", __FILE__, __LINE__, __FUNCTION__)
#else
#define compilerrt_abort() abort()
#endif

#if !defined(INFINITY) && defined(HUGE_VAL)
#define INFINITY HUGE_VAL
#endif /* INFINITY */

typedef      int si_int;
typedef unsigned su_int;

typedef          long long di_int;
typedef unsigned long long du_int;

typedef union
{
    di_int all;
    struct
    {
#if _YUGA_LITTLE_ENDIAN
        su_int low;
        si_int high;
#else
        si_int high;
        su_int low;
#endif /* _YUGA_LITTLE_ENDIAN */
    }s;
} dwords;

typedef union
{
    du_int all;
    struct
    {
#if _YUGA_LITTLE_ENDIAN
        su_int low;
        su_int high;
#else
        su_int high;
        su_int low;
#endif /* _YUGA_LITTLE_ENDIAN */
    }s;
} udwords;

#if __x86_64

typedef int      ti_int __attribute__ ((mode (TI)));
typedef unsigned tu_int __attribute__ ((mode (TI)));

typedef union
{
    ti_int all;
    struct
    {
#if _YUGA_LITTLE_ENDIAN
        du_int low;
        di_int high;
#else
        di_int high;
        du_int low;
#endif /* _YUGA_LITTLE_ENDIAN */
    }s;
} twords;

typedef union
{
    tu_int all;
    struct
    {
#if _YUGA_LITTLE_ENDIAN
        du_int low;
        du_int high;
#else
        du_int high;
        du_int low;
#endif /* _YUGA_LITTLE_ENDIAN */
    }s;
} utwords;

static inline ti_int make_ti(di_int h, di_int l) {
    twords r;
    r.s.high = h;
    r.s.low = l;
    return r.all;
}

static inline tu_int make_tu(du_int h, du_int l) {
    utwords r;
    r.s.high = h;
    r.s.low = l;
    return r.all;
}

#endif /* __x86_64 */

typedef union
{
    su_int u;
    float f;
} float_bits;

typedef union
{
    udwords u;
    double  f;
} double_bits;

typedef struct
{
#if _YUGA_LITTLE_ENDIAN
    udwords low;
    udwords high;
#else
    udwords high;
    udwords low;
#endif /* _YUGA_LITTLE_ENDIAN */
} uqwords;

typedef union
{
    uqwords     u;
    long double f;
} long_double_bits;

#endif /* INT_LIB_H */

