/*
 * minedl_elf wrapper methods.
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

#ifndef MINEDL_ELF_HH_
#define MINEDL_ELF_HH_

#include <stdint.h>

#include <mine/minedl_basic.hh>

class minedl_elf_t : public minedl_basic_t {

	void * _dl_handler;

	string _name;

	/* forbid copy */
	minedl_elf_t(minedl_elf_t const &);
	/* forbid assign */
	minedl_elf_t & operator=(minedl_elf_t const &);

public:
	/* create lib */
	minedl_elf_t(char const * name);
	/* close lib */
	virtual ~minedl_elf_t();
	/* get addr of symbol */
	virtual intptr_t dlsym(string const & name) const;
	/* Execute object */
	virtual int execve(char *const argv[], char *const envp[]) const;

};


#endif /* MINEDL_ELF_HH_ */
