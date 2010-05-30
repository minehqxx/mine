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
#include <mine/common.h>
#include "wrapper.h"
/***********************************************************************
 * Functions
 ***********************************************************************/

/* 
* http://developer.apple.com/mac/library/documentation/DeveloperTools/gcc-4.0.1/gccint/Integer-library-routines.html
*/

/* GGC_S linux wrapper */
extern di_int __udivdi3(di_int a, di_int b);

di_int
mine_udivdi3(di_int a, di_int b){
	debug__("(%s) : %d, %d",__FUNCTION__, a, b );
	return __udivdi3(a,b);
}
