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
#include "reader_macho.hh"

/***********************************************************************
 * Types
 ***********************************************************************/
/***********************************************************************
 * Variables
 ***********************************************************************/
/***********************************************************************
 * Local Prototypes
 ***********************************************************************/
/***********************************************************************
 * Functions
 ***********************************************************************/


reader_macho_t::reader_macho_t(char const * in_pc_filename) :
	_name(in_pc_filename)
	{
	read();
}

reader_macho_t::~reader_macho_t() {
	//TODO destructor.
}

int reader_macho_t::read() {
	int ec;


	/* Load macho and generate events with verbose */
	ec = this->load(_name.c_str(), true);
	if (ec != EC_SUCCESS) {
		crit___("Unable to read MAch-O file: %s", _name.c_str());
		goto out_err;
	}

	ec = EC_SUCCESS;
	out_err: return ec;
}

int reader_macho_t::mach_header_event(mach_header const * hdr) {
	return EC_SUCCESS;
}

int reader_macho_t::new_segment_event(segment_command const * sh) {
	return EC_SUCCESS;
}

int reader_macho_t::new_section_event(section const * sh) {
	return EC_SUCCESS;
}

int reader_macho_t::new_dylib_event(dylib_command const * dh) {
	return EC_SUCCESS;
}

int reader_macho_t::new_symtab_event(symtab_command const * sh) {
	return EC_SUCCESS;
}

int reader_macho_t::new_dysymtab_event(dysymtab_command const * ps_cmd) {
	return EC_SUCCESS;
}

int reader_macho_t::end_of_file_event() {
	return EC_SUCCESS;
}

