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

#ifndef SYSTEM_STAT_H_
#define SYSTEM_STAT_H_
/***********************************************************************
 * Includes
 ***********************************************************************/
#include <mine/common.h>
#include <mine/cdefs.h>

#include <System/types.h>
#include <time.h>

struct darwin_stat {
	__darwin_dev_t st_dev; /* [XSI] ID of device containing file */
	__darwin_mode_t st_mode; /* [XSI] Mode of file (see below) */
	__uint16_t st_nlink; /* [XSI] Number of hard links */
	__darwin_ino64_t st_ino; /* [XSI] File serial number */
	__darwin_uid_t st_uid; /* [XSI] User ID of the file */
	__darwin_gid_t st_gid; /* [XSI] Group ID of the file */
	__darwin_dev_t st_rdev; /* [XSI] Device ID */
	struct timespec st_atimespec;		/* time of last access */
	struct timespec st_mtimespec;		/* time of last data modification */
	struct timespec st_ctimespec;		/* time of last status change */
	struct timespec st_birthtimespec;	/* time of file creation(birth) */
	__darwin_off_t st_size; /* [XSI] file size, in bytes */
	__darwin_blkcnt_t st_blocks; /* [XSI] blocks allocated for file */
	__darwin_blksize_t st_blksize; /* [XSI] optimal blocksize for I/O */
	__uint32_t st_flags; /* user defined flags for file */
	__uint32_t st_gen; /* file generation number */
	__int32_t st_lspare; /* RESERVED: DO NOT USE! */
	__int64_t st_qspare[2]; /* RESERVED: DO NOT USE! */
};

MINEAPI int mine_stat(const char *path, struct darwin_stat *buf) __MINE_INODE64(stat);
MINEAPI int mine_fstat(int fd, struct darwin_stat *buf);
MINEAPI int _fstat$INODE64(int fd, struct darwin_stat *buf);
MINEAPI int _fstat(int fd, struct darwin_stat *buf);

MINEAPI int mine_lstat(const char *path, struct darwin_stat *buf)__MINE_INODE64(lstat);


#endif /* SYSTEM_STAT_H_ */
