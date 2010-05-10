/*
 * elf dl wrapper methods.
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

#include <dlfcn.h>
#include <mine/common.h>
#include <mine/minedl_elf.hh>

minedl_elf_t::minedl_elf_t(char const * name) :
_name(name) {
	debug__("CREATE MEMORY IMAGE FOR %s", name);
	_dl_handler = ::dlopen(name, RTLD_NOW);
	if(_dl_handler == NULL) {
		crit___("Unable to dlopen %s : %s", name, ::dlerror());
	}

	/* Trying to find mine_init in wrapper */
	uintptr_t i_addr = (uintptr_t)::dlsym(_dl_handler, "__mine_init");
	if(i_addr) {
		typedef void (*pf_mine_init_t)(void);
		pf_mine_init_t pf_mine_init = (pf_mine_init_t) i_addr;

		/* Executing wrapper init */
		pf_mine_init();


	}

}
/* close lib */
minedl_elf_t::~minedl_elf_t() {
	::dlclose(_dl_handler);
}
/* get addr of symbol */
intptr_t minedl_elf_t::dlsym(string const & name) const {
	//debug__("Looking for ELF symbol '%s' in %s(%p)", name.c_str(), _name.c_str(), _dl_handler);
	//return (uintptr_t)::dlsym(_dl_handler, name.c_str());
	uintptr_t i_addr = (uintptr_t)::dlsym(_dl_handler, name.c_str());
	//debug__("%p %s", i_addr, name.c_str());
	return i_addr;
}

int minedl_elf_t::execve(char *const argv[], char *const envp[]) const {
	return EC_FAILURE;
}

