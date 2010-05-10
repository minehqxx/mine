/*
 * Mine misc functions.
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

#include <mine/dl.h>

/**
 * Get file extension from string
 *
 * \param[in] str_file File path
 *
 * \return a string
 */
string misc_get_ext(string str_file) {
	string::size_type idx;

	idx = str_file.rfind('.');

	if(idx != string::npos)
	{
	    return str_file.substr(idx+1);
	}

	return "";
}

/**
 * Get filename from path
 *
 * \param[in] str_file File path
 *
 * \return a string
 */
string misc_get_filename(string str_path) {
	string::size_type idx;

	idx = str_path.rfind('/');

	if(idx != string::npos)
	{
	    return str_path.substr(idx+1);
	}

	return str_path;
}

