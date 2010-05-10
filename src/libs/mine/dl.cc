/*
 * Mine dynamic loader program.
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
#include <mine/dl.h>
#include <mine/minedl.hh>
#include <mine/misc.hh>

/**
 * Opening Mach-O library
 *
 * \param[in] in_pc_filename File path that will be loaded
 * \param[in] in_pi_flag Load options
 *
 * \return a DLL handler
 */

void *
mine_dlopen (const char * in_pc_filename, int in_pi_flag)
{
	void * ec;

	minedl_basic_t * pc_lib;


	/* Load binary as symbol table */
	pc_lib = minedl_t::dlopen(in_pc_filename);
	if (!pc_lib) {
		crit___("Unable to load Mach-0 binary/library");
		ec = NULL;
		goto out_err;
	}

	ec = (void*) pc_lib;


out_err:
	return ec;
}

/**
 * Get current loading error.
 *
 * \return an internal char buffer that contains the error
 */
const char *
mine_dlerror(void)
{
	fixme__("");
	return NULL;
}

/**
 * Get a symbol handler of the loaded library
 *
 * \param[in] in_pv_handle Handler allocated during mine_dlopen()
 * \param[in] in_pc_symbol Name of the symbol
 *
 * \return an internal char buffer that contains the error
 */
void *
mine_dlsym(void * in_pv_handle, char * in_pc_symbol)
{
	void * pv_ec = NULL;
	minedl_t * pc_lib = (minedl_t *) in_pv_handle;
	int ec;
	uintptr_t i_addr;

	i_addr = pc_lib->dlsym(in_pc_symbol);
	if(!i_addr) {
		ec = EC_FAILURE;
		crit___("Unable to locate %s", in_pc_symbol);
		pv_ec = NULL;
		goto out_err;
	}

	pv_ec = (void*) i_addr;

	debug__("%s is at %p", in_pc_symbol, pv_ec);
out_err:
	return pv_ec;
}

/**
 * Close the
 *
 * \param[in] in_pv_handle Handler allocated during mine_dlopen()
 * \param[in] in_pc_symbol Name of the symbol
 *
 * \return an internal char buffer that contains the error
 */
int
mine_dlclose (void * in_pv_handle)
{
	fixme__("%p", in_pv_handle);

	return FALSE;
}

/**
 * Execute the Library
 *
 */
int
mine_execve (const char *in_pc_filename, char *const argv[], char *const envp[])
{
	int ec;

	minedl_basic_t * pc_lib;

	/* Load binary as symbol table */
	pc_lib = minedl_t::dlopen(in_pc_filename);
	if (!pc_lib) {
		crit___("Unable to load Mach-0 binary/library");
		ec = NULL;
		goto out_err;
	}



	debug__("");

	ec = pc_lib->execve(argv, envp);

out_err:
	return ec;
}
