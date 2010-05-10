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


#ifndef MINE_CDEFS_H_
#define MINE_CDEFS_H_

/***********************************************************************
 * Includes
 ***********************************************************************/
/***********************************************************************
 * Macros
 ***********************************************************************/
#define	__MINE_STRING(x)	#x

#define __MINE_SUF_INODE64		"$INODE64"
#define __MINE_SUF_UNIX2003     "$UNIX2003"

#define __MINE_INODE64(sym)		__asm("_" __MINE_STRING(sym) __MINE_SUF_INODE64)
#define __MINE_UNIX2003(sym)    __asm("_" __MINE_STRING(sym) __MINE_SUF_UNIX2003)
#define __MINE_SYM(sym)		    __asm("_" __MINE_STRING(sym) )

#define MINEAPI //__attribute__((__stdcall__))

#endif /* CDEFS_H_ */
