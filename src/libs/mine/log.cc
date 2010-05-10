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

#include <mine/log.hh>

static log_t _default_log(cerr);

/**
 * Trace debug function.
 * Lock debug mutex and print FILE, LINE, FUNCTION info
 * with associated format and values
 *
 * \param[in] file file name where debug function is called
 * \param[in] line line number of file where debug function is called
 * \param[in] func function name where debug function is called
 * \param[in] type type of trace displayed (ERROR, INFO, ...)
 * \param[in] format trace string
 */
void log_t::println(char const * in_str_file, int in_i_line,
		char const * in_str_func, int in_i_level, char const * format, ...) {
	va_list ap;
	va_start(ap, format);
	this->_vprintln(in_str_file, in_i_line, in_str_func, in_i_level, format, ap);
	va_end(ap);
}

/**
 * Trace debug function using va list.
 * Lock debug mutex and print FILE, LINE, FUNCTION info
 * with associated format and values
 *
 * \param[in] file file name where debug function is called
 * \param[in] line line number of file where debug function is called
 * \param[in] func function name where debug function is called
 * \param[in] type type of trace displayed (ERROR, INFO, ...)
 * \param[in] format trace string
 * \param[in] others arguments using va_list
 */
void log_t::_vprintln(const char * in_str_file, int in_i_line,
		const char * in_str_func, int in_i_level, const char *format,
		va_list ap) {
	int i_nb_char = 0;
	int ec;
	char str_type[50];
	char const * pstr_type;

	switch (in_i_level) {
	case log_t::FATAL:
		pstr_type = "FATA";
		break;
	case log_t::CRITICAL:
		pstr_type = "CRIT";
		break;
	case log_t::WARNING:
		pstr_type = "WARN";
		break;
	case log_t::FIXME:
		pstr_type = "FIXM";
		break;
	case log_t::IMPORTANT:
		pstr_type = "IMPO";
		break;
	case log_t::INFO:
		pstr_type = "INFO";
		break;
	default:
		sprintf(&str_type[0], "%d", in_i_level);
		pstr_type = &str_type[0];
		break;
	}

	i_nb_char += snprintf(_buffer, _buffer_size, "%s:%s:%d:",
			pstr_type, in_str_func, in_i_line);

	/* print format with args */
	vsnprintf(&_buffer[i_nb_char], _buffer_size - i_nb_char, format,
			ap);

	/* print new line */
	_out << _buffer << endl;
}

/**
 * Trace debug function.
 * Lock debug mutex and print FILE, LINE, FUNCTION info
 * with associated format and values
 *
 * \param[in] file file name where debug function is called
 * \param[in] line line number of file where debug function is called
 * \param[in] func function name where debug function is called
 * \param[in] type type of trace displayed (ERROR, INFO, ...)
 * \param[in] format trace string
 */
void println(char const * in_str_file, int in_i_line,
		char const * in_str_func, int in_i_level, char const * format, ...) {
	va_list ap;
	int ec;
	/* Call va debug function */
	va_start(ap, format);
	_default_log._vprintln(in_str_file, in_i_line, in_str_func, in_i_level, format, ap);
	va_end(ap);
}
