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

/***********************************************************************
 * Includes
 ***********************************************************************/

#define __USE_POSIX2 1
/* internal includes */
#include <mine/dl.h>
#include "main.h"
/* external include */
#include <dlfcn.h>
#include <unistd.h>
/***********************************************************************
 * Globals
 ***********************************************************************/
struct ST_MINE gs_mine;

/***********************************************************************
 * Functions
 ***********************************************************************/

/**
 * Display usage of mine.
 *
 */
void f_mine_usage(void)
{
	  fprintf( stderr,
			  "Usage: ./mine [OPTIONS] [Mach-O binary] [arguments] \n"
			  " -h\tshow this help\n"
			  " -v\tbe verbose\n"
	           );

}

/**
 * Fill mine configuration
 *
 */
int f_mine_fill_config(struct ST_MINE * in_ps_mine, int in_i_argc, char ** in_ac_argv)
{
	int ec;
	int c;

	while( (c = getopt( in_i_argc, in_ac_argv, "vh"))!= EOF) {

	    switch( c ) {
	    case 'v':
	    	in_ps_mine->b_verbose = 1;
			break;
	    case 'h':
			f_mine_usage();
			ec = EC_FAILURE;
			goto out_err;
			break;
	    case '?':
	      if ( isprint (optopt) ) {
	        fatal__("unrecognised option \"%c\"",(char) optopt );
	        ec = EC_FAILURE;
	        goto out_err;
	      }
	      break;
	    default:
	    	fatal__("unrecognised command line option");
	    	ec = EC_FAILURE;
	    	goto out_err;
	      break;
	    }

	  }

	  /* take first residual non option argv element as program name. */
	  if ( optind < in_i_argc ) {
	    in_ps_mine->str_file = in_ac_argv[ optind ];
	    gs_mine.i_file_index = optind;
	  }

	  /* TODO arguments */

	ec = EC_SUCCESS;
out_err:
	return ec;
}
/**
 * Main function of program
 *
 * \param[in] argc Number of string provided into argv
 * \param[in] argv String array
 *
 * \return
 * - EXIT_SUCCESS on success
 * - EXIT_FAILURE on failure
 */
extern char ** environ;
int main(int in_i_argc, char ** in_ac_argv)
{
	int ec;
	void * ps_lib;
	char ** myargv;

	/* Initialize program from command line */
	ec = f_mine_fill_config(&gs_mine, in_i_argc, in_ac_argv);
	if (ec != EC_SUCCESS) {
		crit___("Unable to set mine configuration");
		ec = EXIT_FAILURE;
		goto out_err;
	}

	myargv = (char**) malloc(sizeof(char*) *(in_i_argc - gs_mine.i_file_index +1));
	if(!myargv) {
		fatal__("Unable to malloc arguments");
		goto out_err;
	}
#if 0
	for(int i=0; i<in_i_argc; i++) {
		debug__("- (%d)%s",i, in_ac_argv[i]);
	}
	debug__("%d %d",gs_mine.i_file_index, in_i_argc - gs_mine.i_file_index);
#endif

	for(int i=0; i<in_i_argc - gs_mine.i_file_index; i++) {
		myargv[i] = in_ac_argv[i+gs_mine.i_file_index];
		debug__("- (%d)%s", i+gs_mine.i_file_index,in_ac_argv[i+gs_mine.i_file_index]);
	}
	myargv[in_i_argc - gs_mine.i_file_index] = NULL;

	/* Load binary as symbol table */
	ec = mine_execve(gs_mine.str_file, (char * const *) myargv, environ);
	if(ec != EC_SUCCESS) {
		crit___("Unable to execute Mach-0 binary/library: %s", mine_dlerror());
		ec = EXIT_FAILURE;
		goto out_err;
	}

	ec = EXIT_FAILURE;
out_err:
	return ec;
}
