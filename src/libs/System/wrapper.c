/*
 * Wrapper Gcc_s library
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

#define _XOPEN_SOURCE
#include <locale.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <unistd.h>
#include <sys/types.h>
#include <getopt.h>
#include <string.h>
#include <stdint.h>
#include <signal.h>



#include <mine/common.h>
//#include <mine/cdefs.h>

#include <System/stdio.h>

/***********************************************************************
 * Functions
 ***********************************************************************/

/* Wrapper init function */
void __mine_init(void) {
	mine_init_findfp();
}

/* Dont know where to put those below functions */
ssize_t
    _listxattr(const char *path, char *namebuf, size_t size, int options) {
	fixme__("(%p,%p,%d,%d) always NULL", path, namebuf, size, options);
	return 0;
}

