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
/***********************************************************************
 * Includes
 ***********************************************************************/
#include <mine/common.h>
#include <mine/minedl_macho.hh>
#include <mine/misc.hh>
#include <mine/dl.h>
#include <sys/mman.h>
#include <mine/log.hh>
#include <mine/minedl.hh>

#include <exception>

#include <cerrno>
#include <cstring>

void minedl_macho_t::_test_fixme(int32_t ths, int32_t idx) {
	debug__("Call undef symbol : %s", (char *)idx);
}

void minedl_macho_t::_trace_native(intptr_t _ths, intptr_t _idx, intptr_t edi, intptr_t esi, intptr_t ebp, intptr_t ebx, intptr_t edx, intptr_t ecx, intptr_t eax, intptr_t esp) {
	minedl_macho_t * ths = (minedl_macho_t *) _ths;
	int32_t idx = _idx;
	/* should it have only one item ? */
	symtab_command const * symcmd = ths->_lst_symtabs.front();
	/* should it have only one item ? */
	dysymtab_command const * dysymcmd = ths->_lst_dysymtabs.front();
	struct nlist * nl = (struct nlist *) (ths->_pc_image + symcmd->symoff);
	char * str = (char *) (ths->_pc_image + symcmd->stroff);
	int32_t * ps_indirectsym = (int32_t *) (ths->_pc_image
			+ dysymcmd->indirectsymoff);
	int32_t i_tmp = ths->_s_jump_table->reserved1 + idx;
	struct nlist * s_sym = nl + ps_indirectsym[i_tmp];
	string str_symbol = str + s_sym->n_un.n_strx;
	debug__("Trace call native [%s] of [%s]", str_symbol.c_str(), ths->_name.c_str());
	debug__("Trace call native register eax:%08x ecx:%08x edx:%08x ebx:%08x", eax, ecx, edx, ebx);
	debug__("Trace call native register esp:%08x ebp:%08x esi:%08x edi:%08x", esp, ebp, esi, edi);
}

int32_t minedl_macho_t::_create_trace(intptr_t ths, intptr_t idx,
		trace_func_t fnc, int32_t jmp) {
	/* get free space for trace function */
	__call_trace_t * ptr = _func_trace_mem.new_func();
	x86::push_register(ptr->push_esp, REG_SP);
	x86::push_register(ptr->push_eax, REG_A);
	x86::push_register(ptr->push_ecx, REG_C);
	x86::push_register(ptr->push_edx, REG_D);
	x86::push_register(ptr->push_ebx, REG_B);
	x86::push_register(ptr->push_ebp, REG_BP);
	x86::push_register(ptr->push_esi, REG_SI);
	x86::push_register(ptr->push_edi, REG_DI);
	x86::push(ptr->push_idx, (int32_t) idx);
	x86::push(ptr->push_ths, ths);
	x86::call(ptr->call_func, (int32_t) fnc - (int32_t) &(ptr->sub_esp));
	x86::sub_esp(ptr->sub_esp, 8);
	x86::pop_register(ptr->pop_edi, REG_DI);
	x86::pop_register(ptr->pop_esi, REG_SI);
	x86::pop_register(ptr->pop_ebp, REG_BP);
	x86::pop_register(ptr->pop_ebx, REG_B);
	x86::pop_register(ptr->pop_edx, REG_D);
	x86::pop_register(ptr->pop_ecx, REG_C);
	x86::pop_register(ptr->pop_eax, REG_A);
	x86::pop_register(ptr->pop_esp, REG_SP);
	x86::jump(ptr->jump_func, jmp - ((int32_t) &(ptr->jump_func)
			+ sizeof(x86::jump_t)));
	return (int32_t) ptr;
}

int32_t minedl_macho_t::_create_callback(int32_t ths, int32_t idx,
		call_back_func fnc) {
	__call_fixme_t * ptr = _func_fixme_mem.new_func();
	x86::push_register(ptr->push_ebp, REG_BP);
	x86::mov_esp_to_ebp(ptr->mov_esp_to_ebp);
	x86::push(ptr->push_idx, idx);
	x86::push(ptr->push_ths, ths);
	x86::call(ptr->call_func, (int32_t) fnc - (int32_t) &(ptr->sub_esp));
	x86::sub_esp(ptr->sub_esp, 8);
	x86::leave(ptr->leave);
	x86::ret(ptr->ret);
	return (int32_t) ptr;
}

minedl_macho_t::minedl_macho_t(char const * in_pc_filename) :
	_name(in_pc_filename) {
	loader_start(in_pc_filename);
}

minedl_macho_t::~minedl_macho_t() {
	//TODO destructor.
}

int minedl_macho_t::loader_start(char const * in_pc_filename) {
	int ec;
	uint32_t i_jump_offset = 0;

	_s_jump_table = 0;
	_s_pointers = 0;

	/* Load macho and generate events */
	ec = this->load(in_pc_filename);
	if (ec != EC_SUCCESS) {
		crit___("Unable to read MAch-O file: %s", in_pc_filename);
		goto out_err;
	}

#if 0
	//_m_syms["main"] = 0x1fca;
	void (*pf_main)(int in_i_argc, char const ** in_pc_argv);
	pf_main = (void(*)(int, char const **)) (_m_syms["_main"]);
	D("Ececuting %p", pf_main);
	{
		char const x[] = "toto";
		char const * y[1];
		y[0] = x;
		pf_main(1, y);
	}
	D("");
#endif
	ec = EC_SUCCESS;
	out_err: return ec;
}

int minedl_macho_t::mach_header_event(mach_header const * hdr) {
	/* Saving header */
	_s_header = *hdr;
	return EC_SUCCESS;
}

int minedl_macho_t::new_segment_event(segment_command const * sh) {
	int ec;
	char * pc_mem;
	/* register segments */
	_lst_segments.push_back(sh);
	ec = EC_SUCCESS;
	out_err: return ec;
}

int minedl_macho_t::new_section_event(section const * sh) {

	int ec;

	/* delay memcpy */
	_lst_sections.push_back(sh);

	if (string(sh->sectname) == "__jump_table") {
		_s_jump_table = sh;
	} else if (string(sh->sectname) == "__pointers") {
		_s_pointers = sh;
	}

	ec = EC_SUCCESS;
	out_err: return ec;
}

int minedl_macho_t::new_dylib_event(dylib_command const * dh) {
	_lst_dylibs.push_back(dh);
	return EC_SUCCESS;
}

int minedl_macho_t::new_symtab_event(symtab_command const * sh) {
	_lst_symtabs.push_back(sh);
	return EC_SUCCESS;
}

/* load symbol table and save undef symbol */
int minedl_macho_t::new_dysymtab_event(dysymtab_command const * sh) {
	_lst_dysymtabs.push_back(sh);
	return EC_SUCCESS;
}

int minedl_macho_t::resolve_sym(string const & in_str_symbol,
		intptr_t & out_i_addr) const {
	int ec = EC_FAILURE;
	//debug__("Try to find %s", in_str_symbol.c_str());

	out_i_addr = 0;

	/* Find into current object */
	out_i_addr = dlsym(in_str_symbol);
	if (out_i_addr)
		return EC_SUCCESS;

	/* Find into direct sub libs
	 * research should not be recursive, probably */
	for (list<minedl_basic_t *>::const_iterator it = _lst_libs.begin(); it
			!= _lst_libs.end(); it++) {
		out_i_addr = (*it)->dlsym(in_str_symbol);
		if (out_i_addr) {
			return EC_SUCCESS;
		}
	}
	return EC_FAILURE;
}

/* Find into current object */
intptr_t minedl_macho_t::dlsym(string const & s_symbol) const {
	//debug__("Looking for MACH-0 symbol '%s' ", s_symbol.c_str());
	map<string, intptr_t>::const_iterator const it = _m_syms.find(s_symbol);
	if (it == _m_syms.end()) {
		//warn___("Symbol '%s' not found", s_symbol.c_str());
		return 0;
	} else {
		//debug__("Symbol '%s' found in %s", s_symbol.c_str(), _name.c_str());
		return _pc_mem + it->second;
	}
}

int minedl_macho_t::update_link_tables(void) {
	int ec;

	/* should it have only one item ? */
	symtab_command const * symcmd = _lst_symtabs.front();
	/* should it have only one item ? */
	dysymtab_command const * dysymcmd = _lst_dysymtabs.front();

	struct nlist * nl = (struct nlist *) (_pc_image + symcmd->symoff);
	char * str = (char *) (_pc_image + symcmd->stroff);
	uint32_t * ps_indirectsym = (uint32_t *) (_pc_image
			+ dysymcmd->indirectsymoff);

	/* Jump table */
	if (_s_jump_table) {
		x86::jump_t * pc_tmp = (x86::jump_t *) (_pc_mem + _s_jump_table->addr);

		for (int i = 0; i * 5 < _s_jump_table->size; i++) {

			intptr_t i_addr = _create_lazy_bind((int32_t) this, i, _lazy_bind,
					(int32_t) &(pc_tmp[i]));
			/* Write x86 JMP */
			x86::jump(pc_tmp[i], i_addr - ((int32_t) &pc_tmp[i + 1]));
		}
	}

	/* Pointer table */
	if (_s_pointers) {
		int32_t * pc_tmp = (int32_t *) (_pc_mem + _s_pointers->addr);
		for (int i = 0; i * sizeof(int32_t) < _s_pointers->size; i++) {
			int32_t i_tmp = _s_pointers->reserved1 + i;
			uint32_t i_indirectsym = ps_indirectsym[i_tmp];
			if (!(i_indirectsym & (INDIRECT_SYMBOL_ABS | INDIRECT_SYMBOL_LOCAL))) {
				struct nlist *s_sym = nl + i_indirectsym;
				string str_symbol = str + s_sym->n_un.n_strx;
				intptr_t i_addr;

				/* Get symbol address */
				if (s_sym->n_value) {
					i_addr = s_sym->n_value;
				} else {
					ec = resolve_sym(str_symbol, i_addr);
					if (ec != EC_SUCCESS) {
						crit___("Unable to resolve object %s", str_symbol.c_str());
						exit(-1);
						i_addr = 0;
					}
					debug__("new pointers entry: %s", str_symbol.c_str());

				}
				/* Write addr inside pointer table */

				//debug__("%p", i_addr);
				//if (str_symbol == "__strerrp") {

				//}
				pc_tmp[i] = i_addr;
			}

		}
	}
	ec = EC_SUCCESS;
	out_err: return ec;
}

int32_t minedl_macho_t::_create_lazy_bind(int32_t ths, int32_t idx,
		lazy_bind_func_t fnc, int32_t jmp) {
	/* get free executable space for __call_lazy_bind_t function */
	__call_lazy_bind_t * ptr = _func_lazy_bind_mem.new_func();
	/* initialise the lazybind function */
	x86::push_register(ptr->push_esp, REG_SP);
	x86::push_register(ptr->push_eax, REG_A);
	x86::push_register(ptr->push_ecx, REG_C);
	x86::push_register(ptr->push_edx, REG_D);
	x86::push_register(ptr->push_ebx, REG_B);
	x86::push_register(ptr->push_ebp, REG_BP);
	x86::push_register(ptr->push_esi, REG_SI);
	x86::push_register(ptr->push_edi, REG_DI);
	x86::push(ptr->push_idx, idx);
	x86::push(ptr->push_ths, ths);
	x86::call(ptr->call_func, (int32_t) fnc - (int32_t) &(ptr->sub_esp));
	x86::sub_esp(ptr->sub_esp, 8);
	x86::pop_register(ptr->pop_edi, REG_DI);
	x86::pop_register(ptr->pop_esi, REG_SI);
	x86::pop_register(ptr->pop_ebp, REG_BP);
	x86::pop_register(ptr->pop_ebx, REG_B);
	x86::pop_register(ptr->pop_edx, REG_D);
	x86::pop_register(ptr->pop_ecx, REG_C);
	x86::pop_register(ptr->pop_eax, REG_A);
	x86::pop_register(ptr->pop_esp, REG_SP);
	x86::jump(ptr->jump_func, jmp - ((int32_t) &(ptr->jump_func)
			+ sizeof(x86::jump_t)));
	return (int32_t) ptr;
}

void minedl_macho_t::_lazy_bind(intptr_t _ths, intptr_t _idx) {
	int ec;
	int32_t i = (int32_t) _idx;
	minedl_macho_t * ths = (minedl_macho_t *) _ths;
	/* should it have only one item ? */
	symtab_command const * symcmd = ths->_lst_symtabs.front();
	/* should it have only one item ? */
	dysymtab_command const * dysymcmd = ths->_lst_dysymtabs.front();

	struct nlist * nl = (struct nlist *) (ths->_pc_image + symcmd->symoff);
	char * str = (char *) (ths->_pc_image + symcmd->stroff);
	int32_t * ps_indirectsym = (int32_t *) (ths->_pc_image
			+ dysymcmd->indirectsymoff);

	x86::jump_t * pc_tmp = (x86::jump_t *) (ths->_pc_mem
			+ ths->_s_jump_table->addr);

	int32_t i_tmp = ths->_s_jump_table->reserved1 + i;
	uint32_t i_indirectsym = ps_indirectsym[i_tmp];

	if (!(i_indirectsym & (INDIRECT_SYMBOL_ABS | INDIRECT_SYMBOL_LOCAL))) {
		struct nlist * s_sym = nl + i_indirectsym;
		string str_symbol = str + s_sym->n_un.n_strx;
		debug__("Lazy bind of %s", str_symbol.c_str());
		intptr_t i_addr;
		/* Get symbol address */
		if (s_sym->n_value) {
			i_addr = s_sym->n_value;
		} else {
			ec = ths->resolve_sym(str_symbol, i_addr);
			if (ec != EC_SUCCESS) {
				crit___("Unable to resolve %s", str_symbol.c_str());
				i_addr = ths->_create_callback((int32_t) ths, (int32_t) (str
						+ s_sym->n_un.n_strx), _test_fixme);

				exit(-1);
			}
		}

		i_addr = ths->_create_trace(_ths, _idx, _trace_native, i_addr);
		/* Re-Write x86 JMP */
		x86::jump(pc_tmp[i], i_addr - ((int32_t) &pc_tmp[i + 1]));
	}
}

int minedl_macho_t::new_entrypoint_event(uintptr_t in_i_addr) {
	_i_entry = in_i_addr;
	return EC_SUCCESS;
}

int minedl_macho_t::end_of_file_event() {
	int ec;

	debug__("CREATE MEMORY IMAGE FOR %s", _name.c_str());

	/* create mmap image */
	uintptr_t min_addr = 0xFFFFFFFF;
	uintptr_t max_addr = 0;
	for (list<segment_command const *>::const_iterator it =
			_lst_segments.begin(); it != _lst_segments.end(); it++) {
		segment_command const * sh = *it;
		/* ignore PAGEZERO, this is a security page. */
		if (string(sh->segname) == "__PAGEZERO") {
			//debug__("Found __PAGEZERO");
			continue;
		}
		if (sh->vmaddr < min_addr)
			min_addr = sh->vmaddr;
		if (sh->vmaddr + sh->vmsize > max_addr)
			max_addr = sh->vmaddr + sh->vmsize;
	}

	switch (_s_header.filetype) {
	case MH_EXECUTE:
		debug__("New MMAP: @0x%08x(%d)", min_addr, max_addr - min_addr);
		/* we choose the place */
		_pc_mem = (intptr_t) mmap((void*) min_addr, max_addr - min_addr,
				PROT_WRITE | PROT_READ | PROT_EXEC, MAP_ANONYMOUS | MAP_PRIVATE
						| MAP_FIXED, -1, 0);
		if (((intptr_t) _pc_mem) == -1) {
			crit___("Unable to mmap: %s", strerror(errno));
			return EC_FAILURE;
		}
		_pc_mem = 0;
		break;
	case MH_DYLIB:
		debug__("New MMAP: @mmap_choose(%d)", max_addr - min_addr);
		/* mmap will choose offset */
		_pc_mem = (intptr_t) mmap(0, max_addr - min_addr, PROT_WRITE
				| PROT_READ | PROT_EXEC, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
		if (((intptr_t) _pc_mem) == -1) {
			crit___("Unable to mmap: %s", strerror(errno));
			return EC_FAILURE;
		}
		break;
	default:
		_pc_mem = -1;
	}

	/* copy segments */
	for (list<section const *>::iterator it = _lst_sections.begin(); it
			!= _lst_sections.end(); it++) {
		section const * sh = *it;
		if ((sh->flags & SECTION_TYPE) == S_ZEROFILL) {
			debug__("memset @0x%08x (%d)", (_pc_mem + sh->addr), sh->size);
			memset((void*) (_pc_mem + sh->addr), 0, sh->size);
		} else {
			debug__("memcpy @0x%08x from 0x%08x(%d)", (_pc_mem + sh->addr), sh->offset, sh->size);
			memcpy((void*) (_pc_mem + sh->addr), (void*) (_pc_image
					+ sh->offset), sh->size);
		}
	}

	/* reads symbol list */
	if (_lst_symtabs.size() != 1)
		warn___("Size of %d for symtab is not expected!", _lst_symtabs.size());
	/* should it have only one item ? */
	symtab_command const * symcmd = _lst_symtabs.front();

	if (_lst_dysymtabs.size() != 1)
		warn___("Size of %d for dysymtab is not expected!", _lst_dysymtabs.size());
	/* should it have only one item ? */
	dysymtab_command const * dysymcmd = _lst_dysymtabs.front();
	struct nlist * nl = (struct nlist *) (_pc_image + symcmd->symoff);
	char * str = (char *) (_pc_image + symcmd->stroff);
	for (int i = 0; i < symcmd->nsyms; i++) {
		int i_strid = nl[i].n_un.n_strx;
		string sym_name = &str[i_strid];
		//debug__("Converting: %s %x (section:%d) %d", sym_name.c_str(), nl[i].n_value, nl[i].n_sect, nl[i].n_type & N_EXT);
		if ((nl[i].n_type & N_TYPE) != N_UNDF) {
			_m_syms[sym_name] = nl[i].n_value;
		} else {
			_l_undefsyms.push_front(sym_name);
		}
	}

	/* load sub libs */
	for (list<dylib_command const *>::iterator it = _lst_dylibs.begin(); it
			!= _lst_dylibs.end(); it++) {
		dylib_command const * sh = *it;
		char * ch = (char *) sh;
		string str_mine_lib = ch + sh->dylib.name.offset;
		//debug__("Try to open '%s'", str_mine_lib.c_str());
		minedl_basic_t * pv_handler = minedl_t::dlopen(str_mine_lib.c_str());
		if (!pv_handler) {
			crit___("Library '%s' not found", str_mine_lib.c_str());
			ec = EC_FAILURE;
		} else {
			debug__("Found '%s' library", str_mine_lib.c_str());
			_lst_libs.push_back(pv_handler);
			ec = EC_SUCCESS;
		}
	}

	ec = update_link_tables();
	if (ec != EC_SUCCESS) {
		crit___("Unable to update Link tables");
		goto out_err;
	}

	debug__("RELEASE MEMORY IMAGE FOR %s", _name.c_str());

	ec = EC_SUCCESS;
	out_err: return ec;
}

int minedl_macho_t::execve(char * const argv[], char * const envp[]) const {
	typedef int (*pf_entry_t)(int argc, char ** argv);

	pf_entry_t pf_entry = (pf_entry_t) ((char*) _i_entry);

	debug__("Execve: %p (%p %p)", pf_entry, argv, envp);

	/* Count argument */
	size_t sz_argv = 0;
	size_t sz_envp = 0;

	while (argv[sz_argv]) {
		//debug__("%s",argv[sz_argv] );
		sz_argv++;
	}
	while (envp[sz_envp])
		sz_envp++;

	//debug__("%p %s", &argv[0], argv[0]);
	__asm__(
			// ENVP PUSH LOOP
			"add    $0x1, %3;"
			"_loop_envp:"
			"push   (%2);"
			"sub    $0x4, %2;"
			"sub    $0x1, %3;"
			"test   %3,%3;"
			"jne _loop_envp;"

			// ARGV PUSH LOOP
			"mov    %1,%3;"
			"add    $0x1, %3;"
			"_loop_argv:"
			"push   (%0);"
			"sub    $0x4, %0;"
			"sub    $0x1, %3;"
			"test   %3,%3;"
			"jne _loop_argv;"

			// PUSH ARGC
			"push   %1;"

			// JUMP TO ENTRY
			"jmp    *(%4);"
			:
			: "r"((&argv[sz_argv])), "r"(sz_argv), "r"((&envp[sz_envp])), "r" (sz_envp), "r"(&pf_entry)
			:
	);

	fatal__("We should never reach this !!!!");

	return EC_SUCCESS;
}
