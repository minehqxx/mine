/*
 * Mine log facilities.
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

#ifndef LOG_HH_
#define LOG_HH_

#ifdef __cplusplus
extern "C" {
#endif
void println(const char * in_str_file, int in_i_line, const char * in_str_func,
		int in_i_level, const char *format, ...);
#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
#include <iostream>
#include <stdarg.h>

using namespace std;

class log_t {
	ostream & _out;
	char * _buffer;
	int const _buffer_size;

	void _vprintln(char const * in_str_file, int in_i_line,
			char const * in_str_func, int in_i_level, char const *format,
			va_list ap);

	/* copy is not allowed */
	log_t(log_t const &);

	/* assign is not allowed */
	log_t & operator=(log_t const &);

public:
	log_t() :
		_out(cout), _buffer_size(4096) {
		_buffer = new char[_buffer_size];
		/* little seccurity */
		_buffer[_buffer_size - 1] = 0;
	}

	log_t(ostream & out) :
		_out(out), _buffer_size(4096) {
		_buffer = new char[_buffer_size];
		/* little seccurity */
		_buffer[_buffer_size - 1] = 0;
	}

	virtual ~log_t() {
		delete[] _buffer;
	}

	enum {
		/** trace types **/
		/** fatal error - debug message type */
		FATAL = (-5),
		/** error - debug message type */
		CRITICAL = (-4),
		/** import - debug message type */
		FIXME = (-3),
		/** warning - debug message type */
		WARNING = (-2),
		/** import - debug message type */
		IMPORTANT = (-1),
		/** info - debug message type */
		INFO = 0,
		/* #define USER1 "user1" */
		USER1 = 1,
		/* #define USER2 "user2" */
		USER2 = 2,
	};

	void println(const char * in_str_file, int in_i_line,
			const char * in_str_func, int in_i_level, const char *format, ...);

	friend void ::println(const char * in_str_file, int in_i_line,
			const char * in_str_func, int in_i_level, const char *format, ...);

};


#define debug__(format, ...) println(__FILE__, __LINE__, __FUNCTION__, log_t::INFO,  format, ##__VA_ARGS__)
#define crit___(format, ...) println(__FILE__, __LINE__, __FUNCTION__, log_t::CRITICAL,  format, ##__VA_ARGS__)
#define fatal__(format, ...) println(__FILE__, __LINE__, __FUNCTION__, log_t::FATAL,  format, ##__VA_ARGS__)
#define fixme__(format, ...) println(__FILE__, __LINE__, __FUNCTION__, log_t::FIXME,  format, ##__VA_ARGS__)
#define warn___(format, ...) println(__FILE__, __LINE__, __FUNCTION__, log_t::WARNING,  format, ##__VA_ARGS__)
#else

enum {
	/** trace types **/
	/** fatal error - debug message type */
	FATAL = (-5),
	/** error - debug message type */
	CRITICAL = (-4),
	/** import - debug message type */
	FIXME = (-3),
	/** warning - debug message type */
	WARNING = (-2),
	/** import - debug message type */
	IMPORTANT = (-1),
	/** info - debug message type */
	INFO = 0,
	/* #define USER1 "user1" */
	USER1 = 1,
	/* #define USER2 "user2" */
	USER2 = 2,
};

#define debug__(format, ...) println(__FILE__, __LINE__, __FUNCTION__, INFO,  format, ##__VA_ARGS__)
#define crit___(format, ...) println(__FILE__, __LINE__, __FUNCTION__, CRITICAL,  format, ##__VA_ARGS__)
#define fatal__(format, ...) println(__FILE__, __LINE__, __FUNCTION__, FATAL,  format, ##__VA_ARGS__)
#define fixme__(format, ...) println(__FILE__, __LINE__, __FUNCTION__, FIXME,  format, ##__VA_ARGS__)
#define warn___(format, ...) println(__FILE__, __LINE__, __FUNCTION__, WARNING,  format, ##__VA_ARGS__)

#endif
#endif /* LOG_HH_ */
