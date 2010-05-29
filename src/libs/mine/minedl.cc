/*
 * Macho methods.
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

#include <stdlib.h>
#include <sys/mman.h>

#include <mine/minedl.hh>
#include <mine/log.hh>
#include <mine/minedl_macho.hh>
#include <mine/minedl_elf.hh>

config_t * minedl_t::_default_cfg = NULL;
bool minedl_t::_initialized = false;
string minedl_t::_mine_chroot = "/crazy_buggy_path_";
string minedl_t::_mine_conf_file_name;
map<string, minedl_t *> minedl_t::_already_loaded;

minedl_t::minedl_t(char const * name) :
	_base_name(name) {
	string str_elf = _base_name + ".so";
	string str_macho = _base_name;

	_native_lib = NULL;
	_builtin_lib = NULL;

	//debug__("enterring");
	if (!_initialized) {
		char * chroot = getenv("MINE_CHROOT");
		if (!chroot) {
			crit___("MINE_CHROOT must be set");
			exit(1);
		}

		char const * home = getenv("HOME");
		if (!home) {
			home = ".";
		}

		_mine_conf_file_name = "/home/gschwind";
		_mine_conf_file_name += "/.mine.conf";
		debug__("Load config %s", _mine_conf_file_name.c_str());
		_default_cfg = new config_t(_mine_conf_file_name);
		_mine_chroot = chroot;
		_initialized = true;
	}

	/* Prepend chroot if file not found using local path */
	if (access((str_macho).c_str(), R_OK) != 0) {
		str_macho = _mine_chroot + '/' + str_macho;
	}

	/* Prepend chroot if file not found using local path */
	if (access(str_elf.c_str(), R_OK) != 0) {
		str_elf = _mine_chroot + '/' + str_elf;
	}

	//debug__("Open macho ...");
	if (access(str_macho.c_str(), R_OK) == 0) {
		_native_lib = new minedl_macho_t(str_macho.c_str());
	}

	//debug__("Open builtin ...");
	if (access(str_elf.c_str(), R_OK) == 0) {
		_builtin_lib = new minedl_elf_t(str_elf.c_str());
	}

	if ((_native_lib == NULL) && (_builtin_lib == NULL)) {
		warn___("Library %s has not been found", name);
	}

	_order = BUILTIN_FIRST;
	_nb_ref = 1;

	/* register myself */
	_already_loaded[_base_name] = this;

}

minedl_t::~minedl_t() {
	if (_native_lib) {
		delete _native_lib;
		_native_lib = 0;
	}

	if (_builtin_lib) {
		delete _builtin_lib;
		_builtin_lib = 0;
	}

	if (_default_cfg) {
		delete _default_cfg;
		_default_cfg = 0;
	}
}

int minedl_t::execve(char * const argv[], char * const envp[]) const {
	//if (_native_lib)
	return _native_lib->execve(argv, envp);
}

intptr_t minedl_t::dlsym(string const & name) const {
	uintptr_t sym_addr = 0;
	switch (_order) {
	case NATIVE_FIRST:
		if (_native_lib) {
			sym_addr = _native_lib->dlsym(name);
			if (sym_addr)
				return sym_addr;
		}
		if (_builtin_lib) {
			sym_addr = _builtin_lib->dlsym(name);
			if (sym_addr)
				return sym_addr;
		}
		break;
	case BUILTIN_FIRST:
		if (_builtin_lib) {
			sym_addr = _builtin_lib->dlsym(name);
			if (sym_addr) {
				debug__("Using built in %s@%08x", name.c_str(), sym_addr);
				return sym_addr;
			} else {
				debug__("Nofound builtin %s", name.c_str(), sym_addr);
			}
		}
		if (_native_lib) {
			sym_addr = _native_lib->dlsym(name);
			if (sym_addr)
				return sym_addr;
		}
		break;
	case NATIVE_ONLY:
		if (_native_lib) {
			sym_addr = _native_lib->dlsym(name);
			if (sym_addr)
				return sym_addr;
		}
		break;
	case BUILTIN_ONLY:
		if (_builtin_lib) {
			sym_addr = _builtin_lib->dlsym(name);
			if (sym_addr)
				return sym_addr;
		}
		break;
	}
	return 0;
}

minedl_basic_t * minedl_t::dlopen(char const * name) {
	string s_name(name);
	map<string, minedl_t *>::iterator it(_already_loaded.find(s_name));
	if (it == _already_loaded.end()) {
		return new minedl_t(name);
	} else {
		++(it->second->_nb_ref);
		return it->second;
	}
}

void minedl_t::dlclose(minedl_t * ptr) {
	--(ptr->_nb_ref);
	if (ptr->_nb_ref == 0) {
		map<string, minedl_t *>::iterator it;
		it = _already_loaded.find(ptr->_base_name);
		if (it != _already_loaded.end()) {
			delete ptr;
			_already_loaded.erase(it);
		} else {
			warn___("You havn't use dlopen, are you crazy ???");
		}
	}
}

int minedl_t::operator()(int argc, char * * argv) {
	int (*pf_main)(int in_i_argc, char ** in_ac_argv);
	pf_main = (int(*)(int, char **)) dlsym("_main");
	if (!pf_main) {
		crit___("Unable to find symbol: _main ");
		return EXIT_FAILURE;
	}

	/* Execute Main */
	return (*pf_main)(argc, argv);
}

