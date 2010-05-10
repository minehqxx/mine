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



#ifndef COMMON_H_
#define COMMON_H_

/***********************************************************************
 * Includes
 ***********************************************************************/
/* external include */
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <errno.h>

/* internal includes */
#include "log.hh"

#ifdef __cplusplus
#include <string>
#include <list>
#include <map>
using namespace std;
#endif
/***********************************************************************
 * Defines
 ***********************************************************************/

#define EC_SUCCESS 0
#define EC_FAILURE -1
#define EC_BYPASS -1
#define EC_TIMEOUT -1

/** Define of TRUE state */
#ifndef true
#define true 1
#endif
#ifndef TRUE
#define TRUE true
#endif

/** Define of False state */
#ifndef false
#define false 0
#endif
#ifndef FALSE
#define FALSE false
#endif

/***********************************************************************
 * Macros
 ***********************************************************************/
#define	M_STRINGIFY(x)	#x

/***********************************************************************
 * Types
 ***********************************************************************/
#ifndef __cplusplus
typedef unsigned char bool;
#endif

#endif /* COMMON_H_ */
