/*
 * Macho reader class.
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
#ifndef READER_MACHO_HH_
#define READER_MACHO_HH_

#include <mine/common.h>
#include <mine/macho.hh>


class reader_macho_t : public macho_reader_t {
private:

	string _name;

public:
	reader_macho_t(char const * in_pc_filename);
	~reader_macho_t();
	int read(void);

	/* Virtual methods */
	virtual int mach_header_event(mach_header const * hdr);
	virtual int new_section_event(section const * sh);
	virtual int new_segment_event(segment_command const * sh);
	virtual int new_symtab_event(symtab_command const * sh);
	virtual int new_dysymtab_event(dysymtab_command const * sh);
	virtual int new_dylib_event(dylib_command const * dh);
	virtual int end_of_file_event();

};

#endif /* READER_MACHO_HH_ */
