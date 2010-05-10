/*
 * Wrapper System library
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

/***********************************************************************
 * Includes
 ***********************************************************************/
#include <mine/common.h>
#include <System/unistd.h>

/* Glibc Linux headers */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

MINEAPI int  mine_open
(const char *pathname, int flags, ...) __MINE_UNIX2003(open);

/***********************************************************************
 * Functions
 ***********************************************************************/
MINEAPI int  mine_open
(const char *pathname, int flags, ...) {
	mode_t s_mode;
	debug__("(%s,%x)", pathname, flags);
	va_list ap;
	int ec;
	va_start(ap, flags);
	s_mode = va_arg(ap, mode_t);
	va_end(ap);

	return open(pathname, flags, s_mode);
}
