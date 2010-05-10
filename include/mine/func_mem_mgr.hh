/*
 * This is a manager for function memory.
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
#ifndef FUNC_MEM_MGR_HH_
#define FUNC_MEM_MGR_HH_

#include <list>
#include <cstdint>
#include <sys/mman.h>
#include <mine/log.hh>

using namespace std;

template<typename T, unsigned int BLOCK_SIZE = 0x4000>
class func_mem_mgr_t {
	intptr_t _pc_func_mem_end;
	intptr_t _pc_next_func_mem;
	unsigned int const _align_sizeof_func;
	list<intptr_t> _lst_func_mem;

	func_mem_mgr_t(func_mem_mgr_t const & _);
	func_mem_mgr_t & operator=(func_mem_mgr_t const & _);
public:
	func_mem_mgr_t() :
		_pc_func_mem_end(0), _pc_next_func_mem(0), _align_sizeof_func(
				((sizeof(T) >> 2) << 2) + 4), _lst_func_mem() {
	}

	~func_mem_mgr_t() {
		while (!_lst_func_mem.empty()) {
			munmap((void *) (_lst_func_mem.front()), BLOCK_SIZE);
			_lst_func_mem.pop_front();
		}
	}

	T * new_func() {
		_pc_next_func_mem += _align_sizeof_func;
		if (_pc_next_func_mem >= _pc_func_mem_end) {
			_pc_next_func_mem
					= (intptr_t) mmap(0, BLOCK_SIZE, PROT_WRITE | PROT_READ
							| PROT_EXEC, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
			if ((int32_t) _pc_next_func_mem == -1) {
				fatal__("Cannot map %d o", BLOCK_SIZE);
				exit(1);
			}
			_lst_func_mem.push_back(_pc_next_func_mem);
			_pc_func_mem_end = _pc_next_func_mem + BLOCK_SIZE;
		}
		return (T *)_pc_next_func_mem;
	}
};

#endif /* FUNC_MEM_MGR_HH_ */
