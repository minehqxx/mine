/*
 * Mine dynamic loader header.
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

#ifndef ELF_HH_
#define ELF_HH_

/* This headers come from elf specs 1.1 */

#include <stdint.h>
#include <vector>
#include <iomanip>
#include <iostream>

namespace elf {

std::string hex8_to_string(uint8_t v) {
	std::ostringstream oss;
	oss.fill('0');
	oss << "0x" << std::setw(2) << std::hex << (uint32_t) v;
	return oss.str();
}

std::string hex32_to_string(uint32_t v) {
	std::ostringstream oss;
	oss.fill('0');
	oss << "0x" << std::setw(8) << std::hex << v;
	return oss.str();
}

std::string int_to_index_string(uint32_t i) {
	std::ostringstream oss;
	oss.fill('0');
	oss << "[" << std::setw(3) << i << "]";
	return oss.str();
}

struct ELF32 {
	typedef uint32_t addr_t;
	typedef uint32_t off_t;
	typedef int32_t int_t;
	typedef uint32_t uint_t;

	typedef uint16_t half_t;
	typedef uint32_t word_t;
	typedef uint64_t xword_t;

	typedef int16_t shalf_t;
	typedef int32_t sword_t;
	typedef int64_t sxword_t;

	typedef uint8_t uchar_t;
};

struct ELF64 {
	typedef uint64_t addr_t;
	typedef uint64_t off_t;
	typedef uint64_t int_t;
	typedef int64_t sint_t;

	typedef uint16_t half_t;
	typedef uint32_t word_t;
	typedef uint64_t xword_t;

	typedef int16_t shalf_t;
	typedef int32_t sword_t;
	typedef int64_t sxword_t;
};

typedef uint32_t addr_t;
typedef uint32_t off_t;
typedef int32_t int_t;
typedef uint32_t uint_t;

typedef uint16_t half_t;
typedef uint32_t word_t;
typedef uint64_t xword_t;

typedef int16_t shalf_t;
typedef int32_t sword_t;
typedef int64_t sxword_t;

typedef uint8_t uchar_t;

/* know e_ident index */
enum {
	EI_MAG0 = 0,
	EI_MAG1 = 1,
	EI_MAG2 = 2,
	EI_MAG3 = 3,
	EI_CLASS = 4,
	EI_DATA = 5,
	EI_VERSION = 6,
	EI_PAD = 7,
	EI_NIDENT = 16
};

/* know e_type */
enum {
	ET_NONE = 0, /* no file type */
	ET_REL = 1, /* relocatable */
	ET_EXEC = 2, /* executable file */
	ET_DYN = 3, /* shared object */
	ET_CORE = 4, /* core file */
	ET_LOPROC = 0xff00, /* processor specific */
	ET_HIPROC = 0xffff,
/* processor specific */
};

/* know e_machine */
enum {
	EM_NONE = 0, EM_M32 = 1, EM_SPARC = 2, EM_386 = 3, /* intel x86 */
	EM_68K = 4, EM_88K = 5, EM_860 = 7, EM_MIPS = 8,
};

/* know e_version */
enum {
	/* invalid version */
	EV_NONE = 0,
	/* current version */
	EV_CURRENT = 1,
};

/* elf magic number */
enum {
	ELFMAG0 = 0x7f, ELFMAG1 = 'E', ELFMAG2 = 'L', ELFMAG3 = 'F',
};

/* elf possible class, intel are ELFCLASS32 */
enum {
	ELFCLASSNONE = 0, ELFCLASS32 = 1, ELFCLASS64 = 2,
};

/* elf possible ei_data, intel are ELFDATA2LSB */
enum {
	ELFDATANONE = 0, ELFDATA2LSB = 1, ELFDATA2MSB = 2,
};

enum {
	PT_NULL = 0,
	PT_LOAD = 1,
	PT_DYNAMIC = 2,
	PT_INTERP = 3,
	PT_NOTE = 4,
	PT_SHLIB = 5,
	PT_PHDR = 6,
	PT_LOPROC = 0x70000000,
	PT_HIPROC = 0x7fffffff,
};

enum {
	/* inactive section */
	SHT_NULL = 0,
	SHT_PROGBITS = 1,
	SHT_SYMTAB = 2,
	SHT_STRTAB = 3,
	SHT_RELA = 4,
	SHT_HASH = 5,
	SHT_DYNAMIC = 6,
	SHT_NOTE = 7,
	SHT_NOBITS = 8,
	SHT_REL = 9,
	SHT_SHLIB = 10,
	SHT_DYNSYM = 11,
	SHT_LOPROC = 0x70000000,
	SHT_HIPROC = 0x7fffffff,
	SHT_LOUSER = 0x80000000,
	SHT_HIUSER = 0xffffffff,
};

enum {
	STB_LOCAL = 0,
	STB_GLOBAL = 1,
	STB_WEAK = 2,
	STB_LOPROC = 13,
	STB_HIPROC = 15,
};

std::string get_st_bind_name(uint32_t v) {
	uint32_t vi = ((v) >> 4);
	switch (vi) {
	case STB_LOCAL:
		return std::string("STB_LOCAL  ");
	case STB_GLOBAL:
		return std::string("STB_GLOBAL ");
	case STB_WEAK:
		return std::string("STB_WEAK   ");
	default:
		if (STB_LOPROC <= vi && STB_HIPROC >= vi)
			return std::string("STB_PROC  ");
		return hex8_to_string(vi);
	}
}

enum {
	STT_NOTYPE = 0,
	STT_OBJECT = 1,
	STT_FUNC = 2,
	STT_SECTION = 3,
	STT_FILE = 4,
	STT_LOPROC = 13,
	STT_HIPROC = 15
};

std::string get_st_type_name(uint32_t v) {
	uint32_t vi = ((v) & 0xf);
	switch (vi) {
	case STT_NOTYPE:
		return std::string("STT_NOTYPE  ");
	case STT_OBJECT:
		return std::string("STT_OBJECT  ");
	case STT_FUNC:
		return std::string("STT_FUNC    ");
	case STT_SECTION:
		return std::string("STT_SECTION ");
	case STT_FILE:
		return std::string("STT_FILE    ");
	default:
		if (STT_LOPROC <= vi && STT_HIPROC >= vi)
			return std::string("STT_PROC    ");
		return hex8_to_string(vi);
	}
}

enum {
	SHN_ABS, SHN_COMMON, SHN_UNDEF = 0,
};

enum {
	R_386_NONE = 0,
	R_386_32 = 1,
	R_386_PC32 = 2,
	R_386_GOT32 = 3,
	R_386_PLT32 = 4,
	R_386_COPY = 5,
	R_386_GLOB_DAT = 6,
	R_386_JMP_SLOT = 7,
	R_386_RELATIVE = 8,
	R_386_GOTOFF = 9,
	R_386_GOTPC = 10,
};

std::string get_rel_typename(uint32_t type) {
	switch (type) {
	case R_386_NONE:
		return std::string("R_386_NONE");
	case R_386_32:
		return std::string("R_386_32");
	case R_386_PC32:
		return std::string("R_386_PC32");
	case R_386_GOT32:
		return std::string("R_386_GOT32");
	case R_386_PLT32:
		return std::string("R_386_PLT32");
	case R_386_COPY:
		return std::string("R_386_COPY");
	case R_386_GLOB_DAT:
		return std::string("R_386_GLOB_DAT");
	case R_386_JMP_SLOT:
		return std::string("R_386_JMP_SLOT");
	case R_386_RELATIVE:
		return std::string("R_386_RELATIVE");
	case R_386_GOTOFF:
		return std::string("R_386_GOTOFF");
	case R_386_GOTPC:
		return std::string("R_386_GOTPC");
	default:
		return hex32_to_string(type);
	}
}

std::string get_section_header_typename(word_t type) {
	switch (type) {
	case SHT_NULL:
		return std::string("SHT_NULL");
	case SHT_PROGBITS:
		return std::string("SHT_PROGBITS");
	case SHT_SYMTAB:
		return std::string("SHT_SYMTAB");
	case SHT_STRTAB:
		return std::string("SHT_STRTAB");
	case SHT_RELA:
		return std::string("SHT_RELA");
	case SHT_HASH:
		return std::string("SHT_HASH");
	case SHT_DYNAMIC:
		return std::string("SHT_DYNAMIC");
	case SHT_NOTE:
		return std::string("SHT_NOTE");
	case SHT_NOBITS:
		return std::string("SHT_NOBITS");
	case SHT_REL:
		return std::string("SHT_REL");
	case SHT_SHLIB:
		return std::string("SHT_SHLIB");
	case SHT_DYNSYM:
		return std::string("SHT_DYNSYM");
	default:
		if (type >= 0x70000000 && type <= 0x7fffffff)
			return std::string("SHT_PROC");
		if (type >= 0x80000000 && type <= 0xffffffff)
			return std::string("SHT_USER");
		return hex32_to_string(type);
	}
}

/* the elf header */
struct header_data_t {
	uchar_t ident[EI_NIDENT];
	half_t type;
	half_t machine;
	word_t version;
	addr_t entry;
	off_t phoff;
	off_t shoff;
	word_t flags;
	half_t ehsize;
	half_t phentsize;
	half_t phnum;
	half_t shentsize;
	half_t shnum;
	half_t shstrndx;
};

template<typename T>
union ptr_t {
	T * p;
	uintptr_t i;
};

void print_header(header_data_t * ths) {
	std::cout << "type: " << ths->type << std::endl;
	std::cout << "machine: " << ths->machine << std::endl;
	std::cout << "version: " << ths->version << std::endl;
	std::cout << "entry: " << ths->entry << std::endl;
	std::cout << "phoff: " << ths->phoff << std::endl;
	std::cout << "shoff: " << ths->shoff << std::endl;
	std::cout << "flags: " << ths->flags << std::endl;
	std::cout << "ehsize: " << ths->ehsize << std::endl;
	std::cout << "phentsize: " << ths->phentsize << std::endl;
	std::cout << "phnum: " << ths->phnum << std::endl;
	std::cout << "shentsize: " << ths->shentsize << std::endl;
	std::cout << "shnum: " << ths->shnum << std::endl;
	std::cout << "shstrndx: " << ths->shstrndx << std::endl;
}

/* sections header entry */
struct section_header_entry_t {
	word_t name;
	word_t type;
	int_t flags;
	addr_t addr;
	off_t offset;
	int_t size;
	word_t link;
	word_t info;
	int_t addralign;
	int_t entsize;
};

void print_section_header_hdr() {
	std::cout
			<< "      type          flags      addr                offset            size            link            info       addralign         entsize name"
			<< std::endl;
	//"[000]      SHT_NULL 0x00000000 0x00000000               0               0               0               0               0               0 "
}

void print_section_header(section_header_entry_t * ths, uintptr_t str_offset) {
	std::cout << std::setw(16) << get_section_header_typename(ths->type)
			<< std::setw(16) << hex32_to_string(ths->flags) << std::setw(16)
			<< hex32_to_string(ths->addr) << std::setw(16) << ths->offset
			<< std::setw(16) << ths->size << std::setw(16) << ths->link
			<< std::setw(16) << ths->info << std::setw(16) << ths->addralign
			<< std::setw(16) << ths->entsize << std::setw(16)
			<< reinterpret_cast<char *> (str_offset + ths->name) << std::endl;
}

std::string section_header_to_string(section_header_entry_t * ths,
		uintptr_t str_offset) {
	std::ostringstream oss;
	oss << std::setw(13) << get_section_header_typename(ths->type);
	oss << " " << hex32_to_string(ths->flags);
	oss << " " << hex32_to_string(ths->addr);
	oss << std::setw(16) << ths->offset;
	oss << std::setw(16) << ths->size;
	oss << std::setw(16) << int_to_index_string(ths->link);
	oss << std::setw(16) << ths->info;
	oss << std::setw(16) << ths->addralign;
	oss << std::setw(16) << ths->entsize;
	oss << " " << reinterpret_cast<char *> (str_offset + ths->name);
	return oss.str();
}

/* program header */
struct program_header_entry_t {
	word_t p_type;
	off_t p_offset;
	addr_t p_vaddr;
	addr_t p_paddr;
	word_t p_filesz;
	word_t p_memsz;
	word_t p_flags;
	word_t p_align;
};

std::string print_program_header_type(word_t type) {
	switch (type) {
	case PT_NULL:
		return std::string("PT_NULL");
	case PT_LOAD:
		return std::string("PT_LOAD");
	case PT_DYNAMIC:
		return std::string("PT_DYNAMIC");
	case PT_INTERP:
		return std::string("PT_INTERP");
	case PT_NOTE:
		return std::string("PT_NOTE");
	case PT_SHLIB:
		return std::string("PT_SHLIB");
	case PT_PHDR:
		return std::string("PT_PHDR");
	default:
		if (type >= PT_LOPROC && type <= PT_HIPROC)
			return std::string("PT_PROC");
		return hex32_to_string(type);
	}
}

void print_program_header_hdr(void) {
	std::cout << std::setw(16) << "typename" << std::setw(16) << "type"
			<< std::setw(16) << "offset" << std::setw(16) << "vaddr"
			<< std::setw(16) << "paddr(useless)" << std::setw(16) << "filesz"
			<< std::setw(16) << "memsz" << std::setw(16) << "flags"
			<< std::setw(16) << "align" << std::endl;
}

std::string program_header_to_string(program_header_entry_t * ths) {
	std::ostringstream oss;
	oss << std::setw(16) << print_program_header_type(ths->p_type);
	oss << std::setw(16) << ths->p_offset;
	oss << std::setw(16) << hex32_to_string(ths->p_vaddr);
	oss << std::setw(16) << hex32_to_string(ths->p_paddr);
	oss << std::setw(16) << ths->p_filesz;
	oss << std::setw(16) << ths->p_memsz;
	oss << std::setw(16) << ths->p_flags;
	oss << std::setw(16) << ths->p_align;
	return oss.str();
}

void print_program_header(program_header_entry_t * ths) {
	std::cout << std::setw(16) << print_program_header_type(ths->p_type)
			<< std::setw(16) << hex32_to_string(ths->p_type) << std::setw(16)
			<< ths->p_offset << std::setw(16) << hex32_to_string(ths->p_vaddr)
			<< std::setw(16) << hex32_to_string(ths->p_paddr) << std::setw(16)
			<< ths->p_filesz << std::setw(16) << ths->p_memsz << std::setw(16)
			<< ths->p_flags << std::setw(16) << ths->p_align << std::endl;
}

__inline__ bool is_elf(header_data_t const * ths) {
	return (ths->ident[elf::EI_MAG0] == 0x7f && ths->ident[elf::EI_MAG1] == 'E'
			&& ths->ident[elf::EI_MAG2] == 'L' && ths->ident[elf::EI_MAG3]
			== 'F');
}

__inline__ bool is_lsb(header_data_t const * ths) {
	return ths->ident[elf::EI_DATA] == elf::ELFDATA2LSB;
}

__inline__ int get_class(header_data_t const * ths) {
	return ths->ident[elf::EI_CLASS];
}

/* symbol entry */
struct sym {
	word_t st_name;
	addr_t st_value;
	word_t st_size;
	uchar_t st_info;
	uchar_t st_other;
	half_t st_shndx;
};

void print_symtab_entry_header() {
	std::cout
			<< "      st_value           st_size st_info                  st_other    st_shndx"
			<< std::endl;
	//[000] 0x00000000               0 STB_LOCAL   STT_NOTYPE   0x00               0
}

std::string symtab_entry_to_string(sym * ths) {
	std::ostringstream oss;
	//oss << std::setw(8) << ths->st_name;
	oss << " " << hex32_to_string(ths->st_value);
	oss << std::setw(16) << ths->st_size;
	oss << " " << get_st_bind_name(ths->st_info);
	oss << " " << get_st_type_name(ths->st_info);
	oss << " " << hex8_to_string(ths->st_other);
	oss << std::setw(16) << ths->st_shndx;
	return oss.str();
}

/* relocation entry */
struct rel {
	addr_t r_offset;
	word_t r_info;
};

std::string rel_to_string(rel * ths) {
	std::ostringstream oss;
	oss << hex32_to_string(ths->r_offset);
	oss << std::setw(8) << (ths->r_info >> 8);
	oss << " " << get_rel_typename(ths->r_info & 0xf);
	return oss.str();
}

struct rela {
	addr_t r_offset;
	word_t r_info;
	sword_t r_addend;
};

std::string rela_to_string(rela * ths) {
	std::ostringstream oss;
	oss << hex32_to_string(ths->r_offset);
	oss << std::setw(8) << (ths->r_info >> 8);
	oss << " " << get_rel_typename(ths->r_info & 0xf);
	oss << std::setw(16) << hex32_to_string(ths->r_addend);
	return oss.str();
}

/* program header */
struct p_hdr {
	word_t p_type;
	off_t p_offset;
	addr_t p_vaddr;
	addr_t p_paddr;
	word_t p_filesz;
	word_t p_memsz;
	word_t p_flags;
	word_t p_align;
};

void print_strtab(section_header_entry_t * tmp, ptr_t<header_data_t> hdr) {
	ptr_t<char> i;
	i.i = hdr.i + tmp->offset;
	while (i.i < tmp->offset + hdr.i + tmp->size) {
		if (*(i.p))
			std::cout << *(i.p);
		else
			std::cout << std::endl;
		++i.p;
	}
}

struct dyn {
        sword_t d_tag;
        union {
                 word_t d_val;
                 addr_t d_ptr;
        } d_un;
};

enum {
	DT_NULL = 0,
	DT_NEEDED,
	DT_PLTRELSZ,
	DT_PLTGOT,
	DT_HASH,
	DT_STRTAB,
	DT_SYMTAB,
	DT_RELA,
	DT_RELASZ,
	DT_RELAENT,
	DT_STRSZ,
	DT_SYMENT,
	DT_INIT,
	DT_FINI,
	DT_SONAME,
	DT_RPATH,
	DT_SYMBOLIC,
	DT_REL,
	DT_RELSZ,
	DT_RELENT,
	DT_PLTREL,
	DT_DEBUG,
	DT_TEXTREL,
	DT_JMPREL,
	DT_LOPROC = 0x70000000,
	DT_HIPROC = 0x7fffffff,
};

std::string get_tag_name(word_t type) {
	switch(type) {
	case DT_NULL:
		return std::string("DT_NULL");
	case DT_NEEDED:
		return std::string("DT_NEEDED");
	case DT_PLTRELSZ:
		return std::string("DT_PLTRELSZ");
	case DT_PLTGOT:
		return std::string("DT_PLTGOT");
	case DT_HASH:
		return std::string("DT_HASH");
	case DT_STRTAB:
		return std::string("DT_STRTAB");
	case DT_SYMTAB:
		return std::string("DT_SYMTAB");
	case DT_RELA:
		return std::string("DT_RELA");
	case DT_RELASZ:
		return std::string("DT_RELASZ");
	case DT_RELAENT:
		return std::string("DT_RELAENT");
	case DT_STRSZ:
		return std::string("DT_STRSZ");
	case DT_SYMENT:
		return std::string("DT_SYMENT");
	case DT_INIT:
		return std::string("DT_INIT");
	case DT_FINI:
		return std::string("DT_FINI");
	case DT_SONAME:
		return std::string("DT_SONAME");
	case DT_RPATH:
		return std::string("DT_RPATH");
	case DT_SYMBOLIC:
		return std::string("DT_SYMBOLIC");
	case DT_REL:
		return std::string("DT_REL");
	case DT_RELSZ:
		return std::string("DT_RELSZ");
	case DT_RELENT:
		return std::string("DT_RELENT");
	case DT_PLTREL:
		return std::string("DT_PLTREL");
	case DT_DEBUG:
		return std::string("DT_DEBUG");
	case DT_TEXTREL:
		return std::string("DT_TEXTREL");
	case DT_JMPREL:
		return std::string("DT_JMPREL");
	default:
	 if (DT_LOPROC <= type && DT_HIPROC >= type)
		return std::string("PT_PROC");
	 return hex32_to_string(type);
	}
}


}

#endif /* ELF_HH_ */
