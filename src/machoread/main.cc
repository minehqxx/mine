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
/* external include */
#include <dlfcn.h>

/* internal includes */
#include <mine/dl.h>
#include "main.hh"
#include "reader_macho.hh"

/***********************************************************************
 * Globals
 ***********************************************************************/
struct ST_MACHOREAD gs_machoread;

/***********************************************************************
 * Functions
 ***********************************************************************/

/**
 * Display usage of mine.
 *
 */
void f_machoread_usage(void)
{
	  fprintf( stderr,
			  "Usage: ./machoread [OPTIONS] [Mach-O binary] [arguments] \n"
			  " -h\tshow this help\n"
			  " -v\tbe verbose\n"
	           );

}

/**
 * Fill mine configuration
 *
 */
int f_machoread_fill_config(struct ST_MACHOREAD * in_ps_mine, int in_i_argc, char ** in_ac_argv)
{
	int ec;
	int c;

	while( (c = getopt( in_i_argc, in_ac_argv, "vh"))!= EOF) {

	    switch( c ) {
	    case 'v':
	    	in_ps_mine->b_verbose = 1;
			break;
	    case 'h':
			f_machoread_usage();
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
int main(int in_i_argc, char ** in_ac_argv)
{
	int ec;
	void * ps_lib;
	reader_macho_t * pc_macho_reader = NULL;

	/* Initialize program from command line */
	ec = f_machoread_fill_config(&gs_machoread, in_i_argc, in_ac_argv);
	if(ec != EC_SUCCESS) {
		crit___("Unable to set machoread configuration");
		ec = EXIT_FAILURE;
		goto out_err;
	}

	if(gs_machoread.str_file.size() == 0) {
		f_machoread_usage();
	}

	/* Dump macho information */
	pc_macho_reader = new reader_macho_t(gs_machoread.str_file.c_str());


out_err:
	if(pc_macho_reader) {
		delete(pc_macho_reader);
	}

	return ec;
}
