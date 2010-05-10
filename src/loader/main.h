/*
 * Mine main program.
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

#ifndef MAIN_H_
#define MAIN_H_
/***********************************************************************
 * Includes
 ***********************************************************************/
/* external include */

/* internal includes */
#include <mine/common.h>

/***********************************************************************
 * Types
 ***********************************************************************/

struct ST_MINE
{
	/* Mach-o file */
	char * str_file;

	/* Mach-O file index */
	unsigned short i_file_index;

	/* Verbose mode */
	bool b_verbose;
};


/***********************************************************************
 * Prototypes
 ***********************************************************************/
int mine_fill_config(struct ST_MINE * in_ps_mine, int in_i_argc, char ** in_ac_argv);
void mine_usage(void);



#endif /* MAIN_H_ */
