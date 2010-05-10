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
#ifndef MINEDL_MACHO_HH_
#define MINEDL_MACHO_HH_

#include <list>

using namespace std;

#include <mine/macho.hh>
#include <mine/minedl_basic.hh>
#include <mine/minedl.hh>
#include <mine/x86_struct.hh>
#include <mine/func_mem_mgr.hh>

class minedl_macho_t: public minedl_basic_t, public macho_reader_t {

	typedef void (*call_back_func)(int32_t, int32_t);
	typedef void (*lazy_bind_func_t)(intptr_t, intptr_t);
	typedef void (*trace_func_t)(intptr_t, intptr_t);

	/* template of lazy_bind_func */
	struct __call_lazy_bind_t {
		x86::push_t push_idx;
		x86::push_t push_ths;
		x86::call_t call_func;
		x86::sub_t sub_esp;
		x86::jump_t jump_func;
	}__attribute__ ((__packed__));

	/* template of fixme func */
	struct __call_fixme_t {
		x86::push_ebp_t push_ebp;
		x86::mov_esp_to_ebp_t mov_esp_to_ebp;
		x86::push_t push_idx;
		x86::push_t push_ths;
		x86::call_t call_func;
		x86::sub_t sub_esp;
		x86::leave_t leave; // 0xc9
		x86::ret_t ret; // 0xc3
	}__attribute__((__packed__));

	struct __call_trace_t {
		x86::push_t push_idx;
		x86::push_t push_ths;
		x86::call_t call_func;
		x86::sub_t sub_esp;
		x86::jump_t jump_func;
	}__attribute__ ((__packed__));

	list<segment_command const *> _lst_segments;
	list<section const *> _lst_sections;
	list<dylib_command const *> _lst_dylibs;
	list<symtab_command const *> _lst_symtabs;
	list<dysymtab_command const *> _lst_dysymtabs;

	/* handle memory of __call_lazy_bind_t */
	func_mem_mgr_t<__call_lazy_bind_t> _func_lazy_bind_mem;
	func_mem_mgr_t<__call_fixme_t> _func_fixme_mem;
	func_mem_mgr_t<__call_trace_t> _func_trace_mem;

	/* Jump section */
	section const * _s_jump_table;

	/* Pointers section */
	section const * _s_pointers;

	/* Symbols informations */
	sym_handler_t _s_syms;

	/* header of object */
	mach_header _s_header;

	/* Offset of current mapping */
	intptr_t _i_offset;

	/* Symbols map */
	map<string, intptr_t> _m_syms;
	list<string> _l_undefsyms;

	/* Mem of process section */
	list<intptr_t> _l_mem;

	size_t _sz_mem;

	/* Entry point */
	intptr_t _i_entry;

	/* Library path */
	list<string> _lstr_lib_paths;

	intptr_t _pc_mem;
	string _name;

	int32_t _create_callback(int32_t ths, int32_t idx, call_back_func fnc);
	int32_t _create_lazy_bind(int32_t ths, int32_t idx, lazy_bind_func_t fnc,
			int32_t jmp);
	int32_t _create_trace(intptr_t _ths, intptr_t _idx, trace_func_t fnc,
			int32_t jmp);

	static void _lazy_bind(intptr_t _ths, intptr_t _idx);
	static void _test_fixme(intptr_t ths, intptr_t idx);
	static void _trace_native(intptr_t _ths, intptr_t _idx);

public:
	/* static Library list */
	list<minedl_basic_t *> _lst_libs;

	minedl_macho_t(char const * in_pc_filename);
	virtual ~minedl_macho_t();

	int loader_start(const char * in_pc_filename);
	int update_link_tables(void);
	int resolve_sym(string const & in_str_symbol, intptr_t & out_i_addr) const;

	virtual intptr_t dlsym(string const & in_str_symbol) const;
	virtual int execve(char * const argv[], char * const envp[]) const;

	/* Virtual methods */
	virtual int mach_header_event(mach_header const * hdr);
	virtual int new_section_event(section const * sh);
	virtual int new_segment_event(segment_command const * sh);
	virtual int new_symtab_event(symtab_command const * sh);
	virtual int new_dysymtab_event(dysymtab_command const * sh);
	virtual int new_dylib_event(dylib_command const * dh);
	virtual int new_entrypoint_event(uintptr_t in_i_addr);
	virtual int end_of_file_event();

};

#endif /* MACHO2ELF_H_ */
