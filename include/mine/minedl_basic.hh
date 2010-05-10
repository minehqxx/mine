/*
 * minedl_basic interface methods.
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

#ifndef MINEDL_BASIC_HH_
#define MINEDL_BASIC_HH_

#include <string>

using namespace std;

class minedl_basic_t {
public:
	/* get addr of symbol */
	virtual intptr_t dlsym(string const & name) const = 0;
	virtual int execve(char *const argv[], char *const envp[]) const = 0;
};

#endif /* MINEDL_BASIC_HH_ */
