/*
 * Mine debug methods.
 *
 * Copyright (C) 2010 <Johann Baudy> johann.baudy@gnu-log.net
 * Copyright (C) 2010 <Benoit Gschwind> gschwind@gnu-log.net
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3.0 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#ifndef SYSTEM_STDIO_H_
#define SYSTEM_STDIO_H_
/***********************************************************************
 * Includes
 ***********************************************************************/
#include <mine/common.h>
#include <mine/cdefs.h>

/***********************************************************************
 * Macros
 ***********************************************************************/
/***********************************************************************
 * Variables
 ***********************************************************************/
extern FILE * ___stderrp;
extern FILE * ___stdoutp;
extern FILE * ___stdinp;
/***********************************************************************
 * Functions
 ***********************************************************************/
/* init */
void mine_init_findfp(void);

/* functions */
MINEAPI int mine_puts(const char *s) __MINE_SYM(puts);
MINEAPI int mine_printf(const char *restrict format, ...) __MINE_SYM(printf);
MINEAPI int mine_fprintf(FILE *restrict stream, const char *restrict format,
		...) __MINE_SYM(fprintf);
MINEAPI int mine_snprintf(char *restrict s, size_t n,
		const char *restrict format, ...) __MINE_SYM(snprintf);
MINEAPI int
		mine_sprintf(char *restrict s, const char *restrict format, ...) __MINE_SYM(sprintf);
MINEAPI int
		mine_vfprintf(FILE *restrict stream, const char *restrict format,
				va_list ap) __MINE_SYM(vfprintf);
MINEAPI int
mine_vprintf(const char *restrict format, va_list ap) __MINE_SYM(vprintf);
MINEAPI int
mine_vsnprintf(char *restrict s, size_t n, const char *restrict format,
		va_list ap) __MINE_SYM(vsnprintf);
MINEAPI int
		mine_vsprintf(char *restrict s, const char *restrict format, va_list ap) __MINE_SYM(vsprintf);

MINEAPI FILE * mine_fopen(char const *, char const *) __MINE_SYM(fopen);
MINEAPI int mine_fclose(FILE *) __MINE_SYM(fclose);
MINEAPI size_t mine_fwrite(const void *restrict ptr, size_t size, size_t nitems,
		FILE *restrict stream) __MINE_UNIX2003(fwrite);
MINEAPI size_t mine_fread(void *restrict ptr, size_t size, size_t nitems,
		FILE *restrict stream) __MINE_UNIX2003(fread);

MINEAPI
int mine_fputs(const char *s, FILE *stream) __MINE_UNIX2003(fputs);
MINEAPI
int mine_putchar(int c)  __MINE_SYM(putchar);

MINEAPI int mine_fileno(FILE *stream) __MINE_SYM(fileno);

#endif /* SYSTEM_STDIO_H_ */
