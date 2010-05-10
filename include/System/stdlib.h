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


#ifndef SYSTEM_STDLIB_H_
#define SYSTEM_STDLIB_H_
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
/***********************************************************************
 * Functions
 ***********************************************************************/
MINEAPI void mine_exit(int status) __MINE_SYM(exit);
MINEAPI char * mine_getenv(const char *name) __MINE_SYM(getenv);

MINEAPI void * mine_malloc(size_t size)__MINE_SYM(malloc);
MINEAPI void mine_free(void * ptr)__MINE_SYM(free);
MINEAPI void *mine_calloc(size_t nmemb, size_t size)__MINE_SYM(calloc);
MINEAPI void *mine_realloc(void *ptr, size_t size)__MINE_SYM(realloc);

MINEAPI char * mine_getbsize(int * headerlenp, long *blocksizep)__MINE_SYM(getbsize);

#endif /* SYSTEM_STDLIB_H_ */
