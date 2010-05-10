/*
 * Macho methods C++ version.
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

#ifndef MACHO_HH_
#define MACHO_HH_

#include <cstdint>
#include <cstring>

#include <string>

/***********************************************************************
 * Includes
 ***********************************************************************/
#include <mine/common.h>

typedef unsigned int		__darwin_natural_t;
typedef __darwin_natural_t		natural_t;
#define __DARWIN_UNIX03 1
#include <mach-o/loader.h>
#include <mach-o/fat.h>
#include <mach-o/nlist.h>

/***********************************************************************
 * Types
 ***********************************************************************/
class macho_reader_t {
private:
	int _mach_header(mach_header * ps_mac_header);

	int _new_section(section * in_ps_cmd);

	/* macho commands handlers */
	int _cmd_segment(segment_command * in_pv_cmd);
	int _cmd_symtab(symtab_command * in_ps_symtab);
	int _cmd_symseg(load_command * _);
	int _cmd_thread(load_command * _);
	int _cmd_unixthread(struct thread_command * in_ps_cmd);
	int _cmd_loadfvmlib(load_command * _);
	int _cmd_idfvmlib(load_command * _);
	int _cmd_ident(load_command * _);
	int _cmd_fvmfile(load_command * _);
	int _cmd_prepage(load_command * _);
	int _cmd_dysymtab(dysymtab_command* in_ps_cmd);
	int _cmd_load_dylib(dylib_command * in_ps_dylib);
	int _cmd_id_dylib(load_command * _);
	int _cmd_load_dylinker(load_command * _);
	int _cmd_id_dylinker(load_command * _);
	int _cmd_prebound_dylib(load_command * _);
	int _cmd_routines(load_command * _);
	int _cmd_sub_framework(load_command * _);
	int _cmd_sub_umbrella(load_command * _);
	int _cmd_sub_client(load_command * _);
	int _cmd_sub_library(load_command * _);
	int _cmd_twolevel_hint(load_command * _);
	int _cmd_prebind_cksum(load_command * _);
	int _cmd_load_weak_dylib(load_command * _);
	int _cmd_segment_64(load_command * _);
	int _cmd_routines_64(load_command * _);
	int _cmd_uuid(load_command * _);
	int _cmd_rpath(load_command * _);
	int _cmd_code_signature(load_command * _);
	int _cmd_segment_split_info(load_command * _);
	int _cmd_reexport_dylib(load_command * _);
	int _cmd_lazy_load_dylib(load_command * _);
	int _cmd_encryption_info(load_command * _);
	int _cmd_dyld_info(load_command * _);
	int _cmd_dyld_info_only(load_command * _);
	int _cmd_unknow(load_command * _);

	/* copy constructor */
	macho_reader_t(macho_reader_t const & x);
	/* assign constructor */
	macho_reader_t & operator=(macho_reader_t const & x);

public:
	macho_reader_t() {
	}

	struct sym_handler_t {
		/* Dysym command */
		dysymtab_command * ps_dysymcmd;
		/* symtab command */
		symtab_command * ps_symcmd;
		/* symbol list */
		struct nlist * ps_nlist;
		/* String table */
		char * pc_str;
		/* Indirect symbols table */
		uint32_t * ps_indirectsyms;
	};

	/* Mapped buffer of read file */
	char * _pc_image;

	/* Verbose mode */
	bool _b_verbose;

	/* Symbol table */
	symtab_command * _ps_symcmd;

	string get_file_type_name(int type);

	virtual int load(char const * in_pc_filename, bool in_b_verbose = FALSE);
	/* defaults callbacks */
	virtual int mach_header_event(mach_header const * hdr);
	virtual int new_segment_event(segment_command const * in_pv_cmd);
	virtual int new_section_event(section const * sh);
	virtual int new_symtab_event(symtab_command const * sym);
	virtual int new_dysymtab_event(dysymtab_command const * sym);
	virtual int new_dylib_event(dylib_command const * sym);
	virtual int new_entrypoint_event(uintptr_t in_i_addr);
	virtual int end_of_file_event();

	/* Print functions */
	void print_mach_header(mach_header * ps_mac_header);
	void print_symtab(symtab_command * in_ps_symtab);
	void print_dysymtab(dysymtab_command * in_ps_cmd);
	void print_section(section * in_ps_cmd);
	void print_segment(segment_command * in_ps_cmd);
};

/***********************************************************************
 * Prototypes
 ***********************************************************************/

#endif /* MACHO_HPP_ */
