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

#ifndef SYSTEM_TIME_H_
#define SYSTEM_TIME_H_
/***********************************************************************
 * Includes
 ***********************************************************************/
#include <mine/common.h>
#include <mine/cdefs.h>

#include <time.h>
/***********************************************************************
 * Macros
 ***********************************************************************/
/***********************************************************************
 * Variables
 ***********************************************************************/
/***********************************************************************
 * Functions
 ***********************************************************************/
MINEAPI time_t mine_time(time_t *t) __MINE_SYM(time);
MINEAPI struct tm *mine_localtime(const time_t *timep) __MINE_SYM(localtime);
MINEAPI size_t mine_strftime(char *s, size_t max, const char *format,
		const struct tm *tm) __MINE_UNIX2003(strftime);
#endif /* SYSTEM_TIME_H_ */

