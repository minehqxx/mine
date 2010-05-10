/*
 * Mine debug methods.
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


#ifndef SYSTEM_UNISTD_H_
#define SYSTEM_UNISTD_H_
/***********************************************************************
 * Includes
 ***********************************************************************/
#include <mine/common.h>
#include <mine/cdefs.h>
/***********************************************************************
 * Macros
 ***********************************************************************/
/***********************************************************************
 * Variables
 ***********************************************************************/
extern char *mine_optarg;
/* getopt(3) external variables */
extern int mine_optind __MINE_SYM(optind);
extern int mine_opterr;
extern int mine_optopt;

/***********************************************************************
 * Functions
 ***********************************************************************/
MINEAPI int mine_isatty(int fd) __MINE_SYM(isatty);
MINEAPI uid_t mine_geteuid(void) __MINE_SYM(geteuid);
MINEAPI uid_t mine_getuid(void) __MINE_SYM(getuid);
MINEAPI int mine_getopt(int argc, char * const in_argv[], const char *optstring) __MINE_UNIX2003(getopt);


MINEAPI int
mine_strtofflags(char **stringp, u_long *setp, u_long *clrp) __MINE_SYM(strtofflags);
MINEAPI char *
mine_fflagstostr(u_long flags)__MINE_SYM(fflagstostr);

MINEAPI bool
mine_compat_mode(const char *function, const char *mode)  __MINE_SYM(compat_mode);

ssize_t  mine_readlink(const char * path, char * buf, size_t bufsiz) __MINE_SYM(readlink);

MINEAPI int  mine_close(int) __MINE_UNIX2003(close);

MINEAPI gid_t  mine_getgid(void) __MINE_SYM(getgid);
MINEAPI gid_t  mine_getegid(void) __MINE_SYM(getegid);
MINEAPI int mine_unlink(const char * path) __MINE_SYM(unlink);
#endif /* SYSTEM_UNISTD_H_ */
