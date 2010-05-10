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

#ifndef DL_H_
#define DL_H_


/***********************************************************************
 * Includes
 ***********************************************************************/
#include <mine/common.h>

#ifdef __cplusplus
extern "C" {
#endif
#include <dlfcn.h>
#ifdef __cplusplus
}
#endif

/***********************************************************************
 * Prototypes
 ***********************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

void * mine_dlopen (const char * in_pc_filename, int in_pi_flag);
const char * mine_dlerror(void);
void * mine_dlsym(void * in_pv_handle, char * in_pc_symbol);
int mine_dlclose (void * in_pv_handle);
int mine_execve (const char *path, char *const argv[], char *const envp[]);
#ifdef __cplusplus
}
#endif
#endif /* DL_H_ */
