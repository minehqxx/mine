/*
 * Wrapper System library
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

/* Glibc Linux headers */
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define __DARWIN_UNIX03 1
#define __DARWIN_64_BIT_INO_T 1

/* Darwin headers */
#include <System/stat.h>

static void mine_stat_l2d(struct darwin_stat *out_s_stat, struct stat *in_s_stat ) {
	out_s_stat->st_dev = in_s_stat->st_dev;
	out_s_stat->st_mode = in_s_stat->st_mode; /* [XSI] Mode of file (see below) */
	out_s_stat->st_nlink = in_s_stat->st_nlink; /* [XSI] Number of hard links */
	out_s_stat->st_ino = in_s_stat->st_ino; /* [XSI] File serial number */
	out_s_stat->st_uid = in_s_stat->st_uid; /* [XSI] User ID of the file */
	out_s_stat->st_gid = in_s_stat->st_gid; /* [XSI] Group ID of the file */
	out_s_stat->st_rdev = in_s_stat->st_rdev; /* [XSI] Device ID */
	out_s_stat->st_atimespec.tv_sec = in_s_stat->st_atim.tv_sec;		/* time of last access */
	out_s_stat->st_atimespec.tv_nsec = in_s_stat->st_atim.tv_nsec;		/* time of last access */

	out_s_stat->st_mtimespec.tv_sec = in_s_stat->st_mtim.tv_sec;		/* time of last data modification */
	out_s_stat->st_mtimespec.tv_nsec = in_s_stat->st_mtim.tv_nsec;
	out_s_stat->st_ctimespec.tv_sec = in_s_stat->st_ctim.tv_sec;		/* time of last status change */
	out_s_stat->st_ctimespec.tv_nsec = in_s_stat->st_ctim.tv_nsec;

	out_s_stat->st_birthtimespec.tv_sec = in_s_stat->st_ctim.tv_sec;	/* time of file creation(birth) using ctime ... */
	out_s_stat->st_birthtimespec.tv_nsec = in_s_stat->st_ctim.tv_nsec;

	out_s_stat->st_size = in_s_stat->st_size; /* [XSI] file size, in bytes */
	out_s_stat->st_blocks = in_s_stat->st_blocks; /* [XSI] blocks allocated for file */
	out_s_stat->st_blksize = in_s_stat->st_blksize; /* [XSI] optimal blocksize for I/O */
	out_s_stat->st_flags = 0; /* user defined flags for file */
	out_s_stat->st_gen = 0; /* file generation number */
	out_s_stat->st_lspare = 0; /* RESERVED: DO NOT USE! */
	out_s_stat->st_qspare[0] = 0; /* RESERVED: DO NOT USE! */
	out_s_stat->st_qspare[1] = 0; /* RESERVED: DO NOT USE! */

	//printf("%d %d\n", (int)out_s_stat->st_blocks, (int)out_s_stat->st_blksize);
}

MINEAPI int mine_stat(const char *path, struct darwin_stat *out_s_stat) {
	struct stat s_tmp;
	int ec;

	ec = stat(path, &s_tmp);
	if(ec != 0) {
		goto out_err;
	}

	mine_stat_l2d(out_s_stat, &s_tmp);

out_err:
	return ec;
}

MINEAPI int mine_fstat(int fd, struct darwin_stat *out_s_stat) {
	debug__("fstat: %d %p", fd, out_s_stat);
	struct stat s_tmp;
	int ec;
	ec = fstat(fd, &s_tmp);
	if(ec != 0) {
		goto out_err;
	}
	mine_stat_l2d(out_s_stat, &s_tmp);
out_err:
	return ec;
}

MINEAPI int _fstat$INODE64(int fd, struct darwin_stat *buf) __attribute__ ((weak, alias ("mine_fstat")));
MINEAPI int _fstat(int fd, struct darwin_stat *buf) __attribute__ ((weak, alias ("mine_fstat")));

MINEAPI int mine_lstat(const char *path, struct darwin_stat *out_s_stat) {
	struct stat s_tmp;
	int ec;

	ec = lstat(path, &s_tmp);
	if(ec != 0) {
		goto out_err;
	}

	mine_stat_l2d(out_s_stat, &s_tmp);

out_err:
	return ec;
}


