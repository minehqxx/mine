/*
 * minedl wrapper methods.
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

#ifndef MINEDL_HH_
#define MINEDL_HH_

#include <map>
#include <string>
#include <list>

#include <mine/minedl_basic.hh>
#include <mine/x86_struct.hh>
#include <mine/func_mem_mgr.hh>
#include "config.hh"

using namespace std;

class minedl_t: public minedl_basic_t {

	static config_t * _default_cfg;
	static bool _initialized;
	static string _mine_chroot;
	static string _mine_conf_file_name;
	static map<string, minedl_t *> _already_loaded;

	string _base_name;
	minedl_basic_t * _native_lib;
	minedl_basic_t * _builtin_lib;
	int _nb_ref;

	/* select resolution order */
	enum {
		BUILTIN_FIRST, NATIVE_FIRST, NATIVE_ONLY, BUILTIN_ONLY
	}_order;

	/* forbid copy */
	minedl_t(minedl_t const &);
	/* forbid assign */
	minedl_t & operator=(minedl_t const &);

	/* create lib */
	minedl_t(char const * name);
	/* close lib */
	virtual ~minedl_t();

public:

	/* get addr of symbol */
	virtual intptr_t dlsym(string const & name) const;
	virtual int execve(char * const argv[], char * const envp[]) const;

	static minedl_basic_t * dlopen(char const * name);
	static void dlclose(minedl_t * ptr);

	/* call main */
	int operator()(int argc, char * * argv);

};

#endif /* MINEDL_HH_ */
