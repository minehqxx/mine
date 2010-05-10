/*
 * Configuration handler
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
#include "config.hh"

config_t::config_t(string const & file_name) {
	int eof = 0;
	string buffer;
	ifstream f(file_name.c_str());
	while (getline(f, buffer)) {
		debug__("{%s}", buffer.c_str());
		int begin, end;
		begin = buffer.find("[", 0);
		end = buffer.find("]", begin + 1);
		string key = buffer.substr(0, begin);
		string value = buffer.substr(begin + 1, end - begin - 1);
		debug__("key: <%s> value: <%s>", key.c_str(), value.c_str());
		cfg[key] = value;
	}
}

config_t::~config_t() {

}

string const & config_t::get_config(string const & key, string const & dflt) const {
	std::map<string, string>::const_iterator iter = cfg.find(key);
	if (iter == cfg.end()) {
		return dflt;
	}
	return iter->second;
}
