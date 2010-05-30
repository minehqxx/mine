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

#define sigsegv_outp(x, ...)    debug__(x, ##__VA_ARGS__)

#define SIGSEGV_STACK_X86
#define REGFORMAT "%08x"

static void signal_segv(int signum, siginfo_t * info, void * ptr) {
	static const char *si_codes[3] = { "", "SEGV_MAPERR", "SEGV_ACCERR" };

	int i, f = 0;
	ucontext_t *ucontext = (ucontext_t*) ptr;
	Dl_info dlinfo;
	int32_t bp = 0;
	int32_t ip = 0;

	sigsegv_outp("Segmentation Fault!");
	sigsegv_outp("info.si_signo = %d", signum);
	sigsegv_outp("info.si_errno = %d", info->si_errno);
	sigsegv_outp("info.si_code  = %d (%s)", info->si_code, si_codes[info->si_code]);
	sigsegv_outp("info.si_addr  = %p", info->si_addr);
	for (i = 0; i < NGREG; i++)
		sigsegv_outp("reg[%02d]       = 0x" REGFORMAT, i, ucontext->uc_mcontext.gregs[i]);

	ip = ucontext->uc_mcontext.gregs[REG_EIP];
	bp = ucontext->uc_mcontext.gregs[REG_EBP];

	sigsegv_outp("Stack trace:");
	const char *symname;
	string s, n;
	while (bp && ip) {
		debug__("ip = %08x, bp = %08x", ip, bp);
		if (!dladdr((void *)ip, &dlinfo)) {
			//debug__("dladdr Fail ! : bp = %08x ip = %08x", bp, ip);

			/* find in which lib we are, since we are not in dlopened
			 * function we are in native lib */
			map<string, minedl_t *>::iterator it(minedl_t::_already_loaded.begin());
			int32_t nearest_pc_mem = INT_MIN;
			/* the best candidate */
			minedl_macho_t * nearest_macho;
			while(it != minedl_t::_already_loaded.end()) {
				minedl_macho_t * tmp = (minedl_macho_t *)it->second->_native_lib;
				//debug__("Check %s @ %08x", it->first.c_str(), tmp->_pc_mem);
				if(nearest_pc_mem < tmp->_pc_mem && tmp->_pc_mem < ip) {
					//debug__("Match better %s @ %08x", it->first.c_str(), tmp->_pc_mem);
					nearest_macho = tmp;
					nearest_pc_mem = tmp->_pc_mem;
					n = it->first;
				}
				++it;
			}

			int32_t nearest_function_mem = INT_MIN;
			if(nearest_pc_mem != INT_MIN) {
				map<string, intptr_t>::const_iterator it = nearest_macho->_m_syms.begin();
				while(it != nearest_macho->_m_syms.end()) {
					//sigsegv_outp("%s @ %08x", it->first.c_str(), it->second);
					if(nearest_function_mem < it->second + nearest_pc_mem && it->second + nearest_pc_mem < ip) {
						sigsegv_outp("Match better %s @ %08x", it->first.c_str(), it->second);
						nearest_function_mem = it->second;
						s = it->first;
					}
					++it;
				}
			}
			symname = s.c_str();
			sigsegv_outp("TRACE >>>>>>>>>>>>>>>>>>>>>>>>> %s in %s", symname, n.c_str());
		} else {
			symname = dlinfo.dli_sname;
			sigsegv_outp("% 2d: %p <%s+%lu> (%s)",
					++f,
					ip,
					symname,
					(unsigned long)ip - (unsigned long)dlinfo.dli_saddr,
					dlinfo.dli_fname);

			if (dlinfo.dli_sname && !strcmp(dlinfo.dli_sname, "main"))
				break;
		}

		ip = ((int32_t*)bp)[1];
		bp = ((int32_t*)bp)[0];
	}
	sigsegv_outp("End of stack trace.");
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
