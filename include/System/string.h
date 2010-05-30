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


#ifndef SYSTEM_STRING_H_
#define SYSTEM_STRING_H_
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

MINEAPI void mine_memset(void * dest, int ch, size_t len) __MINE_SYM(memset);
MINEAPI size_t mine_strlen(const char *s) __MINE_SYM(strlen);
MINEAPI char *mine_strcpy(char *dest, const char *src) __MINE_SYM(strcpy);
MINEAPI int mine_strcoll(const char *s1, const char *s2) __MINE_SYM(strcoll);
MINEAPI void * mine_memcpy(void * dest, void * src, size_t len) __MINE_SYM(memcpy);
MINEAPI int mine_strcmp(const char *s1, const char *s2) __MINE_SYM(strcmp);
MINEAPI char * mine_strdup(const char *s1) __MINE_SYM(strdup);
MINEAPI char * mine_strchr(const char * p, int ch) __MINE_SYM(strchr);
MINEAPI char * mine_strrchr(const char * p, int ch) __MINE_SYM(strrchr);
MINEAPI char * mine_index(const char * p, int ch) __MINE_SYM(index);
MINEAPI char * mine_rindex(const char * p, int ch) __MINE_SYM(rindex);


#endif /* SYSTEM_STRING_H_ */


