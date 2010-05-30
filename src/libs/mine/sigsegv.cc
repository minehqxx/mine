/*
 * Mine misc functions.
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

/**
 * Inspired by Jaco Kroon <jaco@kroon.co.za> public domain code. Thanks for his code.
 */

#include <mine/log.hh>
#include <mine/minedl.hh>
#include <mine/minedl_macho.hh>

#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <ucontext.h>
#include <dlfcn.h>
#include <limits.h>

static void signal_segv(int signum, siginfo_t * info, void * ptr) {
	static char const * si_codes[3] = { "", "SEGV_MAPERR", "SEGV_ACCERR" };

	int i, f = 0;
	ucontext_t *ucontext = (ucontext_t*) ptr;
	Dl_info dlinfo;
	int32_t bp = 0;
	int32_t ip = 0;

	debug__("Segmentation Fault");
	debug__("info.si_signo = %d", signum);
	debug__("info.si_errno = %d", info->si_errno);
	debug__("info.si_code  = %d", info->si_code);
	debug__("info.si_addr  = %p", info->si_addr);
	if(info->si_errno)
		debug__("%s", strerror(info->si_errno));

	for (i = 0; i < NGREG; i++)
		debug__("reg[%02d]       = 0x%08x", i, ucontext->uc_mcontext.gregs[i]);

	ip = ucontext->uc_mcontext.gregs[REG_EIP];
	bp = ucontext->uc_mcontext.gregs[REG_EBP];

	debug__("Stack trace:");
	const char *symname;
	string s, n;
	while (bp && ip) {
		debug__("ip = %08x, bp = %08x", ip, bp);
		if (!dladdr((void *) ip, &dlinfo)) {
			/* find in which lib we are, since we are not in dlopened
			 * function we are in native lib */
			map<string, minedl_t *>::const_iterator native_it(
					minedl_t::_already_loaded.begin());
			int32_t nearest_function_mem = INT_MIN;
			/* the best candidate */
			minedl_macho_t * nearest_macho;
			/* For each native libs */
			while (native_it != minedl_t::_already_loaded.end()) {
				minedl_macho_t * tmp =
						(minedl_macho_t *) native_it->second->_native_lib;
				map<string, intptr_t>::const_iterator function_it =
						tmp->_m_syms.begin();
				while (function_it != tmp->_m_syms.end()) {
					//debug__("Check better %s @ %08x", function_it->first.c_str(), function_it->second);
					if (nearest_function_mem < function_it->second + tmp->_pc_mem
							&& function_it->second + tmp->_pc_mem < ip) {
						//sigsegv_outp("Match better %s @ %08x", function_it->first.c_str(), function_it->second);
						nearest_function_mem = function_it->second;
						s = function_it->first;
					}
					++function_it;
				}
				++native_it;
			}

			symname = s.c_str();
			debug__("TRACE >>>>>>>>>>>>>>>>>>>>>>>>> %s in %s", symname, n.c_str());
		} else {
			symname = dlinfo.dli_sname;
			debug__("% 2d: %p <%s+%lu> (%s)",
					++f,
					ip,
					symname,
					(unsigned long)ip - (unsigned long)dlinfo.dli_saddr,
					dlinfo.dli_fname);

			if (dlinfo.dli_sname && !strcmp(dlinfo.dli_sname, "main"))
				break;
		}

		ip = ((int32_t*) bp)[1];
		bp = ((int32_t*) bp)[0];
	}
	debug__("End of stack trace.");
	_exit(-1);
}

static void __attribute__((constructor)) setup_sigsegv() {
	struct sigaction action;
	memset(&action, 0, sizeof(action));
	action.sa_sigaction = signal_segv;
	action.sa_flags = SA_SIGINFO;
	if (sigaction(SIGSEGV, &action, NULL) < 0)
		perror("sigaction");
}
