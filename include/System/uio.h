/*
 * Mine system methods.
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


#ifndef UIO_H_
#define UIO_H_
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
 * Types
 ***********************************************************************/

struct darwin_iovec {
	void *   iov_base;	/* [XSI] Base address of I/O memory region */
	size_t	 iov_len;	/* [XSI] Size of region iov_base points to */
};

/***********************************************************************
 * Functions
 ***********************************************************************/

MINEAPI ssize_t	mine_writev(int, const struct darwin_iovec *, int) __MINE_ALIAS_C(writev);

#endif /* UIO_H_ */
