/*
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
#define XNU_KERNEL_PRIVATE 1
#include <mine/log.hh>
#include <mine/macho.hh>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include <mach-o/reloc.h>

#include <linux/swab.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>

/***********************************************************************
 * Types
 ***********************************************************************/
struct ST_READER_CONTEXT {
	FILE * fd;
	fpos_t s_pos_curr_cmd;
};

/***********************************************************************
 * Functions
 ***********************************************************************/
int macho_reader_t::load(char const * in_pc_filename, bool in_b_verbose) {
	int ec;

	struct stat s_stat;
	void * pv_tmp;
	char * pc_tmp;
	uint32_t *pi_magic;

	/* Store verbose mode */
	_b_verbose = in_b_verbose;

	/* Opening the file */
	int fd = open(in_pc_filename, O_RDONLY);
	if (fd == -1) {
		crit___("Unable to open file %s : %s", in_pc_filename, strerror(errno));
		goto out_err;
	}

	/* Get File size */
	ec = fstat(fd, &s_stat);
	if (ec != 0) {
		crit___("Unable to stat file: %s", in_pc_filename);
		goto out_err;
	}

	/* Check file type */
	if (!S_ISREG (s_stat.st_mode)) {
		crit___("%s is not a file\n", in_pc_filename);
		goto out_err;
	}

	/* Mmap the file into memory */
	//pc_file = (char *) mmap(0, s_stat.st_size, PROT_READ, MAP_SHARED, this->fd,
	//		0);
	/* copy the file in memory */
	_pc_image = (char *) malloc(s_stat.st_size);
	if (!_pc_image) {
		crit___("Unable to open file: %s", strerror(errno));
		goto out_err;
	}
	ec = read(fd, _pc_image, s_stat.st_size);
	close(fd);
	ec = EC_FAILURE;

	debug__("File loaded at %p to %p", _pc_image, _pc_image + s_stat.st_size);

	pi_magic = (uint32_t *) _pc_image;
	pc_tmp = _pc_image;

	if ((*pi_magic == FAT_MAGIC) || (*pi_magic == FAT_CIGAM)) {
		fat_header s_fat_header;
		memcpy(&s_fat_header, pc_tmp, sizeof(s_fat_header));
		pc_tmp += sizeof(s_fat_header);

		/* Check magic word */
		switch (*pi_magic) {
		case FAT_MAGIC:
			break;
		case FAT_CIGAM:
			s_fat_header.magic = __swab32(s_fat_header.magic);
			s_fat_header.nfat_arch = __swab32(s_fat_header.nfat_arch);
			break;
		}

		if (_b_verbose)
			debug__("Fat binary found :%x archs", s_fat_header.nfat_arch);
		for (int i = 0; i < s_fat_header.nfat_arch; i++) {

			fat_arch s_fat_arch;
			memcpy(&s_fat_arch, pc_tmp, sizeof(s_fat_arch));
			pc_tmp += sizeof(s_fat_arch);

			switch (*pi_magic) {
			case FAT_MAGIC:
				break;
			case FAT_CIGAM:
				s_fat_arch.cputype = __swab32(s_fat_arch.cputype);
				s_fat_arch.cpusubtype = __swab32(s_fat_arch.cpusubtype);
				s_fat_arch.offset = __swab32(s_fat_arch.offset);
				s_fat_arch.size = __swab32(s_fat_arch.size);
				s_fat_arch.align = __swab32(s_fat_arch.align);
				break;
			}
			if (_b_verbose)
				debug__("New arch found :%x", s_fat_arch.cputype);

			switch (s_fat_arch.cputype) {
			case CPU_TYPE_X86_64:
				fixme__("CPU_TYPE_X86_64");
				break;
			case CPU_TYPE_X86:
				debug__("CPU_TYPE_X86");
				ec = this->_mach_header((mach_header *) (_pc_image
						+ s_fat_arch.offset));
				if (ec != EC_SUCCESS) {
					crit___("Unable to read mach_header of fat file");
					goto out_err;
				}
				break;
			default:
				crit___("Cpu type %d not supported", s_fat_arch.cputype);
			}
		}
	} else if (*pi_magic == MH_MAGIC) {
		ec = this->_mach_header((mach_header *) _pc_image);
		if (ec != EC_SUCCESS) {
			crit___("Unable to read mach_header of Mach-0 binary");
			goto out_err;
		}
	} else {
		crit___("File format not supported");
		ec = EC_FAILURE;
		goto out_err;
	}

	end_of_file_event();

	ec = EC_SUCCESS;
	out_err: return ec;

}

int macho_reader_t::_mach_header(mach_header * ps_mac_header) {
	int ec;

	char * pc_tmp;
	_pc_image = (char*) ps_mac_header;

	/* call the callback */
	ec = this->mach_header_event(ps_mac_header);
	if (ec != EC_SUCCESS) {
		crit___("Header to accepted by converter");
		goto out_err;
	}

	pc_tmp = (char*) ps_mac_header;
	pc_tmp += sizeof(mach_header);

	if (_b_verbose) {
		print_mach_header(ps_mac_header);
	}

	/* Check magic word */
	if (ps_mac_header->magic != MH_MAGIC) {
		crit___("Wrong magic word");
		ec = EC_FAILURE;
		goto out_err;
	}

	/* Check cpu type */
	switch (ps_mac_header->cputype) {
	case CPU_TYPE_X86:
		break;
	case CPU_TYPE_X86_64:
		fixme__("CPU_TYPE_X86_64");
		ec = EC_FAILURE;
		goto out_err;
		break;
	default:
		crit___("Cpu type not supported: %x", ps_mac_header->cputype);
		ec = EC_FAILURE;
		goto out_err;
	}

	/* Check file type */
	switch (ps_mac_header->filetype) {
	//case MH_OBJECT:			/* relocatable object file */
	case MH_EXECUTE: /* demand paged executable file */
		//		case MH_FVMLIB:			/* fixed VM shared library file */
		//		case MH_CORE:				/* core file */
		//		case MH_PRELOAD:		/* preloaded executable file */
	case MH_DYLIB: /* dynamically bound shared library */
		//		case MH_DYLINKER:			/* dynamic link editor */
		//		case MH_BUNDLE:			/* dynamically bound bundle file */
		//		case MH_DYLIB_STUB:		/* shared library stub for static */
		break;
	default:
		crit___("Filetype not supported: %x", ps_mac_header->filetype);
		break;
	}

	/* Unsupported Flags checking */

	if (ps_mac_header->flags & MH_INCRLINK) {
		fixme__("MH_INCRLINK");
		/* the object file is the output of an
		 incremental link against a base file
		 and can't be link edited again */
		ec = EC_FAILURE;
		goto out_err;
	}

	if (ps_mac_header->flags & MH_BINDATLOAD) {
		fixme__("MH_BINDATLOAD");
		/* the object file's undefined
		 references are bound by the dynamic
		 linker when loaded. */
		ec = EC_FAILURE;
		goto out_err;
	}

	if (ps_mac_header->flags & MH_PREBOUND) {
		fixme__("MH_PREBOUND");
		/* the file has its dynamic undefined
		 references prebound. */
		ec = EC_FAILURE;
		goto out_err;
	}

	if (ps_mac_header->flags & MH_SPLIT_SEGS) {
		fixme__("MH_SPLIT_SEGS");
		/* the file has its read-only and
		 read-write segments split */
		ec = EC_FAILURE;
		goto out_err;
	}

	if (ps_mac_header->flags & MH_LAZY_INIT) {
		fixme__("MH_LAZY_INIT");
		/* the shared library init routine is
		 to be run lazily via catching memory
		 faults to its writeable segments (obsolete)  */
		ec = EC_FAILURE;
		goto out_err;
	}

	if (ps_mac_header->flags & MH_FORCE_FLAT) {
		fixme__("MH_FORCE_FLAT");
		/* the executable is forcing all images
		 to use flat name space bindings */
		ec = EC_FAILURE;
		goto out_err;
	}

	if (ps_mac_header->flags & MH_NOMULTIDEFS) {
		fixme__("MH_NOMULTIDEFS");
		/* this umbrella guarantees no multiple
		 defintions of symbols in its
		 sub-images so the two-level namespace
		 hints can always be used. */
		ec = EC_FAILURE;
		goto out_err;
	}

	if (ps_mac_header->flags & MH_NOFIXPREBINDING) {
		fixme__("MH_NOFIXPREBINDING");
		/* do not have dyld notify the
		 prebinding agent about this
		 executable */
		ec = EC_FAILURE;
		goto out_err;
	}

	if (ps_mac_header->flags & MH_PREBINDABLE) {
		fixme__("MH_PREBINDABLE");
		/* the binary is not prebound but can
		 have its prebinding redone. only used
		 when MH_PREBOUND is not set. */
		ec = EC_FAILURE;
		goto out_err;
	}

	if (ps_mac_header->flags & MH_ALLMODSBOUND) {
		fixme__("MH_ALLMODSBOUND");
		/* indicates that this binary binds to
		 all two-level namespace modules of
		 its dependent libraries. only used
		 when MH_PREBINDABLE and MH_TWOLEVEL
		 are both set. */
		ec = EC_FAILURE;
		goto out_err;
	}

	if (ps_mac_header->flags & MH_SUBSECTIONS_VIA_SYMBOLS) {
		fixme__("MH_SUBSECTIONS_VIA_SYMBOLS");
		/* safe to divide up the sections into
		 sub-sections via symbols for dead
		 code stripping */
		ec = EC_FAILURE;
		goto out_err;
	}

	if (ps_mac_header->flags & MH_CANONICAL) {
		fixme__("MH_CANONICAL");
		/* the binary has been canonicalized
		 via the unprebind operation */
		ec = EC_FAILURE;
		goto out_err;
	}

	if (ps_mac_header->flags & MH_ALLOW_STACK_EXECUTION) {
		fixme__("MH_ALLOW_STACK_EXECUTION");
		/* When this bit is set, all stacks
		 in the task will be given stack
		 execution privilege.  Only used in
		 MH_EXECUTE filetypes. */
		ec = EC_FAILURE;
		goto out_err;
	}
#if 0
	if (ps_mac_header->flags & MH_DEAD_STRIPPABLE_DYLIB) {
		fixme__("MH_DEAD_STRIPPABLE_DYLIB");
		/* Only for use on dylibs.  When
		 linking against a dylib that
		 has this bit set, the static linker
		 will automatically not create a
		 LC_LOAD_DYLIB load command to the
		 dylib if no symbols are being
		 referenced from the dylib. */
		ec = EC_FAILURE;
		goto out_err;
	}
#endif

	if (ps_mac_header->flags & MH_ROOT_SAFE) {
		fixme__("MH_ROOT_SAFE");
		/* When this bit is set, the binary
		 declares it is safe for use in
		 processes with uid zero */
		ec = EC_FAILURE;
		goto out_err;
	}

	if (ps_mac_header->flags & MH_SETUID_SAFE) {
		fixme__("MH_SETUID_SAFE");
		/* When this bit is set, the binary
		 declares it is safe for use in
		 processes when issetugid() is true */
		ec = EC_FAILURE;
		goto out_err;
	}
#if 0
	if (ps_mac_header->flags & MH_PIE) {
		fixme__("MH_PIE");
		/* When this bit is set, the OS will
		 load the main executable at a
		 random address.  Only used in
		 MH_EXECUTE filetypes. */
		ec = EC_FAILURE;
		goto out_err;
	}
#endif
	/*** Parse commands */
	for (int i = 0; i < ps_mac_header->ncmds; i++) {
		struct ST_READER_CONTEXT s_context;
		load_command * ps_tmp = reinterpret_cast<load_command *> (pc_tmp);
		uint32_t i_cmd, i_cmd_size, i_size_tmp;

		switch (ps_tmp->cmd) {
		case LC_SEGMENT:
			/* segment of this file to be mapped */
			this->_cmd_segment(reinterpret_cast<segment_command *> (ps_tmp));
			break;
		case LC_SYMTAB:
			this->_cmd_symtab(reinterpret_cast<symtab_command *> (ps_tmp));
			break; /* link-edit stab symbol table info */
		case LC_SYMSEG:
			this->_cmd_symseg(ps_tmp);
			break; /* link-edit gdb symbol table info (obsolete) */
		case LC_THREAD:
			this->_cmd_thread(ps_tmp);
			break; /* thread */
		case LC_UNIXTHREAD:
			this->_cmd_unixthread((struct thread_command *) ps_tmp);
			break; /* unix thread (includes a stack) */
		case LC_LOADFVMLIB:
			this->_cmd_loadfvmlib(ps_tmp);
			break; /* load a specified fixed VM shared library */
		case LC_IDFVMLIB:
			this->_cmd_idfvmlib(ps_tmp);
			break; /* fixed VM shared library identification */
		case LC_IDENT:
			this->_cmd_ident(ps_tmp);
			break; /* object identification info (obsolete) */
		case LC_FVMFILE:
			this->_cmd_fvmfile(ps_tmp);
			break; /* fixed VM file inclusion (internal use) */
		case LC_PREPAGE:
			this->_cmd_prepage(ps_tmp);
			break; /* prepage command (internal use) */
		case LC_DYSYMTAB:
			/* segment of this file to be mapped */
			this->_cmd_dysymtab((dysymtab_command *) ps_tmp);
			break; /* dynamic link-edit symbol table info */
		case LC_LOAD_DYLIB:
			this->_cmd_load_dylib((dylib_command*) ps_tmp);
			break; /* load a dynamically linked shared library */
		case LC_ID_DYLIB:
			this->_cmd_id_dylib(ps_tmp);
			break; /* dynamically linked shared lib ident */
		case LC_LOAD_DYLINKER:
			this->_cmd_load_dylinker(ps_tmp);
			break; /* load a dynamic linker */
		case LC_ID_DYLINKER:
			this->_cmd_id_dylinker(ps_tmp);
			break; /* dynamic linker identification */
		case LC_PREBOUND_DYLIB:
			this->_cmd_prebound_dylib(ps_tmp);
			break; /* modules prebound for a dynamically */
			/*  linked shared library */
		case LC_ROUTINES:
			this->_cmd_routines(ps_tmp);
			break; /* image routines */
		case LC_SUB_FRAMEWORK:
			this->_cmd_sub_framework(ps_tmp);
			break; /* sub framework */
		case LC_SUB_UMBRELLA:
			this->_cmd_sub_umbrella(ps_tmp);
			break; /* sub umbrella */
		case LC_SUB_CLIENT:
			this->_cmd_sub_client(ps_tmp);
			break; /* sub client */
		case LC_SUB_LIBRARY:
			this->_cmd_sub_library(ps_tmp);
			break; /* sub library */
		case LC_TWOLEVEL_HINTS:
			this->_cmd_twolevel_hint(ps_tmp);
			break; /* two-level namespace lookup hints */
		case LC_PREBIND_CKSUM:
			this->_cmd_prebind_cksum(ps_tmp);
			break; /* prebind checksum */
		case LC_LOAD_WEAK_DYLIB:
			this->_cmd_load_weak_dylib(ps_tmp);
			break;
		case LC_SEGMENT_64: /* 64-bit segment of this file to bemapped */
			this->_cmd_segment_64(ps_tmp);
			break;
		case LC_ROUTINES_64: /* 64-bit image routines */
			this->_cmd_routines_64(ps_tmp);
			break;
		case LC_UUID: /* the uuid */
			this->_cmd_uuid(ps_tmp);
			break;
		case LC_RPATH: /* runpath additions */
			this->_cmd_symseg(ps_tmp);
			break;
		case LC_CODE_SIGNATURE: /* local of code signature */
			this->_cmd_code_signature(ps_tmp);
			break;
		case LC_SEGMENT_SPLIT_INFO: /* local of info to split segments */
			this->_cmd_segment_split_info(ps_tmp);
			break;
		case LC_REEXPORT_DYLIB: /* load and re-export dylib */
			this->_cmd_reexport_dylib(ps_tmp);
			break;
#if 0
			case LC_LAZY_LOAD_DYLIB: /* delay load of dylib until first use */
			this->_cmd_lazy_load_dylib(ps_tmp);
			break;
			case LC_ENCRYPTION_INFO: /* encrypted segment information */
			this->_cmd_encryption_info(ps_tmp);
			break;
			case LC_DYLD_INFO: /* compressed dyld information */
			this->_cmd_dyld_info(ps_tmp);
			break;
			case LC_DYLD_INFO_ONLY: /* compressed dyld information only */
			this->_cmd_dyld_info_only(ps_tmp);
			break;
#endif
		default:
			this->_cmd_unknow(ps_tmp);
			break;
		}

		//debug__("New command: %x %x", ps_tmp->cmd, ps_tmp->cmdsize);

		pc_tmp += ps_tmp->cmdsize;
	}

	ec = EC_SUCCESS;
	goto out_err;

	out_err: return ec;
}

int macho_reader_t::_new_section(section * in_ps_cmd) {
	int ec;
	if (_b_verbose) {
		print_section(in_ps_cmd);
	}

	ec = this->new_section_event(in_ps_cmd);
	if (ec != EC_SUCCESS) {
		crit___("Header to accepted by converter");
		goto out_err;
	}

	ec = EC_SUCCESS;
	out_err: return ec;
}

int macho_reader_t::_cmd_segment(segment_command * in_ps_cmd) {
	int ec;
	if (_b_verbose) {
		print_segment(in_ps_cmd);
	}

	struct section * ps_section = (struct section *) ((char *) in_ps_cmd
			+ sizeof(struct segment_command));

	/* Generate segment event */
	ec = this->new_segment_event(in_ps_cmd);
	if (ec != EC_SUCCESS) {
		crit___("Header to accepted by converter");
		goto out_err;
	}

	for (int i = 0; i < in_ps_cmd->nsects; i++) {
		ec = this->_new_section(ps_section);
		if (ec != EC_SUCCESS) {
			crit___("Unable to read section");
			goto out_err;
		}
		ps_section++;
	}

	ec = EC_SUCCESS;
	out_err: return ec;
}

int macho_reader_t::_cmd_symtab(symtab_command * in_ps_symtab) {
	int ec;
	ec = EC_SUCCESS;

	if (_b_verbose) {
		print_symtab(in_ps_symtab);
	}

	/* Keep symbol table */
	_ps_symcmd = in_ps_symtab;

	new_symtab_event(in_ps_symtab);

	return ec;
}

int macho_reader_t::_cmd_symseg(load_command * _) {
	fixme__("not implemented");
	return EC_SUCCESS;
}

int macho_reader_t::_cmd_thread(load_command * _) {
	fixme__("not implemented");
	return EC_SUCCESS;
}

int macho_reader_t::_cmd_unixthread(struct thread_command * in_ps_cmd) {
	unsigned long *pi_ptr = (unsigned long *) ((char*) in_ps_cmd
			+ sizeof(struct thread_command));

	unsigned long i_flavor = *pi_ptr++;
	unsigned long i_count = *pi_ptr++;
	size_t sz_ptr = in_ps_cmd->cmdsize - sizeof(struct thread_command)
			- sizeof(i_flavor);

	switch (i_flavor) {
	case x86_THREAD_STATE32: {
		i386_thread_state_t * ps_state = (i386_thread_state_t *) pi_ptr;
		debug__("x86_THREAD_STATE32 found:");
		debug__("\teax: %x", ps_state->__eax);
		debug__("\tebx: %x", ps_state->__ebx);
		debug__("\tecx: %x", ps_state->__ecx);
		debug__("\tedx: %x", ps_state->__edx);
		debug__("\tedi: %x", ps_state->__edi);
		debug__("\tesi: %x", ps_state->__esi);
		debug__("\tebp: %x", ps_state->__ebp);
		debug__("\tesp: %x", ps_state->__esp);
		debug__("\tss: %x", ps_state->__ss);
		debug__("\teflags: %x", ps_state->__eflags);
		debug__("\teip: %x", ps_state->__eip);
		debug__("\tcs: %x", ps_state->__cs);
		debug__("\tds: %x", ps_state->__ds);
		debug__("\tes: %x", ps_state->__es);
		debug__("\tfs: %x", ps_state->__fs);
		debug__("\tgs: %x", ps_state->__gs);

		this->new_entrypoint_event((uintptr_t) ps_state->__eip);

		break;
	}
	default:
		crit___("Unknown stack flavor: %x", i_flavor);
		break;

	}

	return EC_SUCCESS;
}

int macho_reader_t::_cmd_loadfvmlib(load_command * _) {
	fixme__("not implemented");
	return EC_SUCCESS;
}

int macho_reader_t::_cmd_idfvmlib(load_command * _) {
	fixme__("not implemented");
	return EC_SUCCESS;
}

int macho_reader_t::_cmd_ident(load_command * _) {
	fixme__("not implemented");
	return EC_SUCCESS;
}

int macho_reader_t::_cmd_fvmfile(load_command * _) {
	fixme__("not implemented");
	return EC_SUCCESS;
}

int macho_reader_t::_cmd_prepage(load_command * _) {
	fixme__("not implemented");
	return EC_SUCCESS;
}

int macho_reader_t::_cmd_dysymtab(dysymtab_command * in_ps_cmd) {
	int ec;

	if (_b_verbose) {
		print_dysymtab(in_ps_cmd);
	}

	this->new_dysymtab_event(in_ps_cmd);

	ec = EC_SUCCESS;
	out_err: return ec;
}

int macho_reader_t::_cmd_load_dylib(dylib_command * in_ps_dylib) {
	int ec;
	ec = EC_SUCCESS;
	debug__("dylib: %s", (char*) in_ps_dylib + in_ps_dylib->dylib.name.offset);

	//	dylib_handler_t s_object_dylib;
	//	s_object_dylib.str_dylib = (char *) in_ps_dylib
	//			+ in_ps_dylib->dylib.name.offset;
	this->new_dylib_event(in_ps_dylib);
	return ec;
}

int macho_reader_t::_cmd_id_dylib(load_command * _) {
	fixme__("not implemented");
	return EC_SUCCESS;
}

int macho_reader_t::_cmd_load_dylinker(load_command * _) {
	fixme__("not implemented");
	return EC_SUCCESS;
}

int macho_reader_t::_cmd_id_dylinker(load_command * _) {
	fixme__("not implemented");
	return EC_SUCCESS;
}

int macho_reader_t::_cmd_prebound_dylib(load_command * _) {
	fixme__("not implemented");
	return EC_SUCCESS;
}

int macho_reader_t::_cmd_routines(load_command * _) {
	fixme__("not implemented");
	return EC_SUCCESS;
}

int macho_reader_t::_cmd_sub_framework(load_command * _) {
	fixme__("not implemented");
	return EC_SUCCESS;
}

int macho_reader_t::_cmd_sub_umbrella(load_command * _) {
	fixme__("not implemented");
	return EC_SUCCESS;
}

int macho_reader_t::_cmd_sub_client(load_command * _) {
	fixme__("not implemented");
	return EC_SUCCESS;
}

int macho_reader_t::_cmd_sub_library(load_command * _) {
	fixme__("not implemented");
	return EC_SUCCESS;
}

int macho_reader_t::_cmd_twolevel_hint(load_command * _) {
	fixme__("not implemented");
	return EC_SUCCESS;
}

int macho_reader_t::_cmd_prebind_cksum(load_command * _) {
	fixme__("not implemented");
	return EC_SUCCESS;
}

/* load a dynamically linked shared library that is allowed to be missing
 * (all symbols are weak imported). */
int macho_reader_t::_cmd_load_weak_dylib(load_command * _) {
	fixme__("not implemented");
	return EC_SUCCESS;
}
int macho_reader_t::_cmd_segment_64(load_command * _) {
	fixme__("not implemented");
	return EC_SUCCESS;
}

int macho_reader_t::_cmd_routines_64(load_command * _) {
	fixme__("not implemented");
	return EC_SUCCESS;
}

int macho_reader_t::_cmd_uuid(load_command * _) {
	fixme__("not implemented");
	return EC_SUCCESS;
}

int macho_reader_t::_cmd_rpath(load_command * _) {
	fixme__("not implemented");
	return EC_SUCCESS;
}

int macho_reader_t::_cmd_code_signature(load_command * _) {
	fixme__("not implemented");
	return EC_SUCCESS;
}

int macho_reader_t::_cmd_segment_split_info(load_command * _) {
	fixme__("not implemented");
	return EC_SUCCESS;
}

int macho_reader_t::_cmd_reexport_dylib(load_command * _) {
	fixme__("not implemented");
	return EC_SUCCESS;
}

int macho_reader_t::_cmd_lazy_load_dylib(load_command * _) {
	fixme__("not implemented");
	return EC_SUCCESS;
}

int macho_reader_t::_cmd_encryption_info(load_command * _) {
	fixme__("not implemented");
	return EC_SUCCESS;
}

int macho_reader_t::_cmd_dyld_info(load_command * _) {
	fixme__("not implemented");
	return EC_SUCCESS;
}

int macho_reader_t::_cmd_dyld_info_only(load_command * _) {
	fixme__("not implemented");
	return EC_SUCCESS;
}

int macho_reader_t::_cmd_unknow(load_command * _) {
	fixme__("not implemented");
	return EC_SUCCESS;
}

int macho_reader_t::mach_header_event(mach_header const * hdr) {
	return EC_SUCCESS;
}

int macho_reader_t::new_segment_event(segment_command const * in_pv_cmd) {
	return EC_SUCCESS;
}

int macho_reader_t::new_section_event(section const * sh) {
	return EC_SUCCESS;
}

int macho_reader_t::new_symtab_event(symtab_command const * sym) {
	return EC_SUCCESS;
}

int macho_reader_t::new_dysymtab_event(dysymtab_command const * sym) {
	return EC_SUCCESS;
}

int macho_reader_t::new_dylib_event(dylib_command const * sym) {
	return EC_SUCCESS;
}

int macho_reader_t::new_entrypoint_event(uintptr_t in_i_addr) {
	return EC_SUCCESS;
}

int macho_reader_t::end_of_file_event() {
	return EC_SUCCESS;
}

string macho_reader_t::get_file_type_name(int type) {

	switch (type) {
	case MH_OBJECT:
		return string("MH_OBJECT");
	case MH_EXECUTE:
		return string("MH_EXECUTE");
	case MH_BUNDLE:
		return string("MH_BUNDLE");
	case MH_DYLIB:
		return string("MH_DYLIB");
	case MH_PRELOAD:
		return string("MH_PRELOAD");
	case MH_CORE:
		return string("MH_CORE");
	case MH_DYLINKER:
		return string("MH_DYLINKER");
	case MH_DSYM:
		return string("MH_DSYM");
	default:
		return string("MH_UNKNOW");
	}
}

void macho_reader_t::print_mach_header(mach_header * ps_mac_header) {
	debug__("");
	debug__("New program header:");
	debug__("- Subtype: %x", ps_mac_header->cpusubtype);
	debug__("- Cputype: %x", ps_mac_header->cputype);
	debug__("- Filetype %s", get_file_type_name(ps_mac_header->filetype).c_str());
	debug__("- Flags: %x", ps_mac_header->flags);
	debug__("- Magic: %x", ps_mac_header->magic);
	debug__("- %d cmds", ps_mac_header->ncmds);
	debug__("- %d bytes", ps_mac_header->sizeofcmds);

	/* Unsupported Flags checking */
	debug__("Flags:");
	if (ps_mac_header->flags & MH_NOUNDEFS) {
		debug__(" -MH_NOUNDEFS");
	}
	if (ps_mac_header->flags & MH_INCRLINK) {
		debug__(" -MH_INCRLINK");
	}
	if (ps_mac_header->flags & MH_DYLDLINK) {
		debug__(" -MH_DYLDLINK");
	}
	if (ps_mac_header->flags & MH_BINDATLOAD) {
		debug__(" -MH_BINDATLOAD");
	}
	if (ps_mac_header->flags & MH_PREBOUND) {
		debug__(" -MH_PREBOUND");
	}
	if (ps_mac_header->flags & MH_SPLIT_SEGS) {
		debug__(" -MH_SPLIT_SEGS");
	}
	if (ps_mac_header->flags & MH_LAZY_INIT) {
		debug__(" -MH_LAZY_INIT");
	}
	if (ps_mac_header->flags & MH_TWOLEVEL) {
		debug__(" -MH_TWOLEVEL");
	}
	if (ps_mac_header->flags & MH_FORCE_FLAT) {
		debug__(" -MH_FORCE_FLAT");
	}
	if (ps_mac_header->flags & MH_NOMULTIDEFS) {
		debug__(" -MH_NOMULTIDEFS");
	}
	if (ps_mac_header->flags & MH_NOFIXPREBINDING) {
		debug__(" -MH_NOFIXPREBINDING");
	}
	if (ps_mac_header->flags & MH_PREBINDABLE) {
		debug__(" -MH_PREBINDABLE");
	}
	if (ps_mac_header->flags & MH_ALLMODSBOUND) {
		debug__(" -MH_ALLMODSBOUND");
	}
	if (ps_mac_header->flags & MH_SUBSECTIONS_VIA_SYMBOLS) {
		debug__(" -MH_SUBSECTIONS_VIA_SYMBOLS");
	}
	if (ps_mac_header->flags & MH_CANONICAL) {
		debug__(" -MH_CANONICAL");
	}
	if (ps_mac_header->flags & MH_WEAK_DEFINES) {
		debug__(" -MH_WEAK_DEFINES");
	}
	if (ps_mac_header->flags & MH_BINDS_TO_WEAK) {
		debug__(" -MH_BINDS_TO_WEAK");
	}
	if (ps_mac_header->flags & MH_ALLOW_STACK_EXECUTION) {
		debug__(" -MH_ALLOW_STACK_EXECUTION");
	}
#if 0
	if (ps_mac_header->flags & MH_DEAD_STRIPPABLE_DYLIB) {
		debug__(" -MH_DEAD_STRIPPABLE_DYLIB");
	}
#endif
	if (ps_mac_header->flags & MH_ROOT_SAFE) {
		debug__(" -MH_ROOT_SAFE");
	}
	if (ps_mac_header->flags & MH_SETUID_SAFE) {
		debug__(" -MH_SETUID_SAFE");
	}
	if (ps_mac_header->flags & MH_NO_REEXPORTED_DYLIBS) {
		debug__(" -MH_NO_REEXPORTED_DYLIBS");
	}
#if 0
	if (ps_mac_header->flags & MH_PIE) {
		debug__(" -MH_PIE");
	}
#endif
}

void macho_reader_t::print_dysymtab(dysymtab_command * in_ps_cmd) {

	debug__("");
	debug__("Dynamic symbols information:");
	debug__("\tnlocalsym:%d nextdefsym:%d "
			"nundefsym:%d nTOC:%d nMOD:%d "
			"nextrefsyms:%d nindirectsyms:%d nextrel:%d nlocrel:%d",
			in_ps_cmd->nlocalsym, in_ps_cmd->nextdefsym, in_ps_cmd->nundefsym,
			in_ps_cmd->ntoc, in_ps_cmd->nmodtab, in_ps_cmd->nextrefsyms,
			in_ps_cmd->nindirectsyms, in_ps_cmd->nextrel, in_ps_cmd->nlocrel);
	debug__("\tilocalsym:%d iextdefsym:%d iundefsym:%d", in_ps_cmd->ilocalsym,
			in_ps_cmd->iextdefsym, in_ps_cmd->iundefsym);

	struct nlist * ps_sym = (struct nlist *) (uintptr_t) (_pc_image
			+ (uintptr_t) _ps_symcmd->symoff);
	char * pc_str = (char*) (uintptr_t) _pc_image
			+ (uintptr_t) _ps_symcmd->stroff;

	struct dylib_table_of_contents * ps_toc =
			(struct dylib_table_of_contents *) (_pc_image + in_ps_cmd->tocoff);
	struct dylib_module * ps_module = (struct dylib_module *) (_pc_image
			+ in_ps_cmd->modtaboff);
	struct dylib_reference * ps_extref = (struct dylib_reference *) (_pc_image
			+ in_ps_cmd->extrefsymoff);
	uint32_t * ps_indirectsym = (uint32_t *) (_pc_image
			+ in_ps_cmd->indirectsymoff);
	struct relocation_info * ps_relocext =
			(struct relocation_info *) (_pc_image + in_ps_cmd->extreloff);
	struct relocation_info * ps_relocloc =
			(struct relocation_info *) (_pc_image + in_ps_cmd->locreloff);

#if 0
	debug__("- Local syms:");
	for (int i = 0; i < in_ps_cmd->nlocalsym; i++) {
		int i_tmp = (ps_sym[in_ps_cmd->ilocalsym + i]).n_un.n_strx;
		debug__("  %s", &pc_str[i_tmp]);
	}

	debug__("- Local syms:");
	for (int i = 0; i < in_ps_cmd->nextdefsym; i++) {
		int i_tmp = (ps_sym[in_ps_cmd->iextdefsym + i]).n_un.n_strx;
		debug__("  %s", &pc_str[i_tmp]);
	}

	debug__("- Undef syms:");
	for (int i = 0; i < in_ps_cmd->nundefsym; i++) {
		int i_tmp = (ps_sym[in_ps_cmd->iundefsym + i]).n_un.n_strx;
		debug__("  %s", &pc_str[i_tmp]);
	}
#endif

	debug__("\t%d TOCs", in_ps_cmd->ntoc);
	for (int i = 0; i < in_ps_cmd->ntoc; i++) {
		debug__("\t\t%d in %d", ps_toc[i].symbol_index, ps_toc[i].module_index);
	}

	debug__("\t%d MODs", in_ps_cmd->nmodtab);
	for (int i = 0; i < in_ps_cmd->nmodtab; i++) {
		debug__("\t\t%s", &(pc_str[ps_module[i].module_name]));
	}

	debug__("\t%d EXTREF", in_ps_cmd->nextrefsyms);
	for (int i = 0; i < in_ps_cmd->nextrefsyms; i++) {
		debug__("\t\t%d(%d)", ps_extref[i].flags, ps_extref[i].isym);
	}

	debug__("\t%d INDIRECT", in_ps_cmd->nindirectsyms);
	for (int i = 0; i < in_ps_cmd->nindirectsyms; i++) {
		debug__("\t\t%x", ps_indirectsym[i]);
	}

	debug__("\t%d EXTREL", in_ps_cmd->nextrel);
	for (int i = 0; i < in_ps_cmd->nextrel; i++) {
		debug__("\t\taddr:%p", ps_relocext[i].r_address);
	}

	debug__("\t%d LOCREL", in_ps_cmd->nlocrel);
	for (int i = 0; i < in_ps_cmd->nlocrel; i++) {
		debug__("\t\taddr:%p", ps_relocloc[i].r_address);
	}
}

void macho_reader_t::print_symtab(symtab_command * in_ps_symtab) {

	//	debug__("cmd:%d cmdsize:%d symoff:%x nsyms:%d stroff:%x strsize:%d ",
	//			in_ps_symtab->cmd,
	//			in_ps_symtab->cmdsize,
	//			in_ps_symtab->symoff,
	//			in_ps_symtab->nsyms, in_ps_symtab->stroff, in_ps_symtab->strsize);

	struct nlist * ps_sym = (struct nlist *) (uintptr_t) (_pc_image
			+ (uintptr_t) in_ps_symtab->symoff);
	char * ac_tmp = (char*) (uintptr_t) _pc_image
			+ (uintptr_t) in_ps_symtab->stroff;

	//debug__("File %p %p", _pc_image, ac_tmp);
	//debug__("Syms %p %p", _pc_image, ps_sym);
	debug__("");
	debug__(" List of Symbols:");
	/* Farse syms */
	for (int i = 0; i < in_ps_symtab->nsyms; i++) {
#if 0
		D("sym:%s n_strx:%d n_type:%d n_sect:%d n_desc:%d n_value:%d",
				ps_sym[i].n_un.n_strx != 0 ? &ac_tmp[ps_sym[i].n_un.n_strx] : "",
				ps_sym[i].n_un.n_strx,
				ps_sym[i].n_un.n_name,
				ps_sym[i].n_type,
				ps_sym[i].n_sect,
				ps_sym[i].n_desc,
				ps_sym[i].n_value
		);
#endif
		//D("%d",i);

		string str_tmp;
		string str_tmp_ref;
		string str_tmp_opt = "";

		switch (ps_sym[i].n_type & N_TYPE) {
		case N_UNDF:
			str_tmp = 'U';
			break;
		case N_ABS:
			str_tmp = 'A';
			break;
		case N_SECT:
			str_tmp = 'T';
			break;
		case N_PBUD:
			str_tmp = 'P';
			break;
		case N_INDR:
			str_tmp = 'I';
			break;
		default:
			continue;
			str_tmp = '?';

		}

		if (ps_sym[i].n_type & N_STAB) {
			str_tmp = "S" + str_tmp;
		}

		if (ps_sym[i].n_type & N_PEXT) {
			str_tmp = "PE" + str_tmp;
		}

		if (ps_sym[i].n_type & N_EXT) {
			str_tmp = "E" + str_tmp;
		}

		switch (ps_sym[i].n_desc & REFERENCE_TYPE) {
		case REFERENCE_FLAG_UNDEFINED_NON_LAZY:
			str_tmp_ref = "UNL";
			break;
		case REFERENCE_FLAG_UNDEFINED_LAZY:
			str_tmp_ref = "UL";
			break;
		case REFERENCE_FLAG_DEFINED:
			str_tmp_ref = "D";
			break;
		case REFERENCE_FLAG_PRIVATE_DEFINED:
			str_tmp_ref = "PD";
			break;
		case REFERENCE_FLAG_PRIVATE_UNDEFINED_LAZY:
			str_tmp_ref = "PUL";
			break;
		case REFERENCE_FLAG_PRIVATE_UNDEFINED_NON_LAZY:
			str_tmp_ref = "PUNL";
			break;
		default:
			str_tmp_ref = "?";
		}

		if (ps_sym[i].n_desc & REFERENCED_DYNAMICALLY) {
			str_tmp_opt += "RD ";
		}
		if (ps_sym[i].n_desc & N_DESC_DISCARDED) {
			str_tmp_opt += "DD ";
		}
#if 0
		if (ps_sym[i].n_desc & N_WEAK_DEF) {
			str_tmp_opt += "WD ";
		}
		if (ps_sym[i].n_desc & N_WEAK_REF) {
			str_tmp_opt += "WR ";
		}
#endif

		debug__("\t %30s\t%03s(%03s)\t%08p\t%s",
				&ac_tmp[ps_sym[i].n_un.n_strx],
				str_tmp.c_str(),
				str_tmp_ref.c_str(),
				ps_sym[i].n_value,
				str_tmp_opt.c_str());
	}
}
void macho_reader_t::print_section(section * in_ps_cmd) {
	string str_flag = "";
	switch (in_ps_cmd->flags & SECTION_TYPE) {
	case S_REGULAR:
		str_flag = "";
		break;
	case S_ZEROFILL:
		str_flag = "ZF";
		break;
	case S_CSTRING_LITERALS:
		str_flag = "STR";
		break;
	case S_4BYTE_LITERALS:
		str_flag = "4B";
		break;
	case S_8BYTE_LITERALS:
		str_flag = "8B";
		break;
	case S_LITERAL_POINTERS:
		str_flag = "P";
		break;
	case S_NON_LAZY_SYMBOL_POINTERS:
		str_flag = "NLSP";
		break;
	case S_LAZY_SYMBOL_POINTERS:
		str_flag = "LAZY_SP";
		break;
	case S_SYMBOL_STUBS:
		str_flag = "SS";
		break;
	case S_MOD_INIT_FUNC_POINTERS:
		str_flag = "MIFP";
		break;
	case S_MOD_TERM_FUNC_POINTERS:
		str_flag = "MTFP";
		break;
	case S_COALESCED:
		str_flag = "C";
		break;
	case S_GB_ZEROFILL:
		str_flag = "GB_ZF";
		break;
	case S_INTERPOSING:
		str_flag = "I";
		break;
	case S_16BYTE_LITERALS:
		str_flag = "16B";
		break;
	default:
		str_flag = "???";
		break;

	}
	debug__("Section name: %30s\t%10s\t%p\t%d\tR:%d\tO:%d\t%d,%d\t%s", in_ps_cmd->sectname,
			in_ps_cmd->segname, in_ps_cmd->addr,
			in_ps_cmd->size,
			in_ps_cmd->nreloc,
			in_ps_cmd->offset,
			in_ps_cmd->reserved1,
			in_ps_cmd->reserved2,
			str_flag.c_str()
			);
}

void macho_reader_t::print_segment(segment_command * in_ps_cmd) {
	if (_b_verbose) {
		debug__("");
		debug__("New Segment: %s at 0x%08x(%d) ", in_ps_cmd->segname, in_ps_cmd->vmaddr,
				in_ps_cmd->vmsize);
	}
}
