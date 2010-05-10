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
#include <System/locale.h>

#include <locale.h>

/***********************************************************************
 * Functions
 ***********************************************************************/

MINEAPI size_t mine_mbrtowc(wchar_t *restrict pwc, const char *restrict s, size_t n,
		mbstate_t *restrict ps) {
	size_t ec;
	debug__("%s(%p,%s,%d,%p)",__FUNCTION__, pwc, s, n, ps);
	ec = mbrtowc(pwc, s, n, NULL);
	//ec = 0;
	debug__("%s", pwc),
	debug__("ec:%d", ec);
	return ec;
}
