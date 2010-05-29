/*
 * Mine x86 facilities.
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

#ifndef X86_STRUCT_HH_
#define X86_STRUCT_HH_

#include <stdint.h>

#define ModRM(mod, rm, reg) ((mod << 6) | (rm << 3) | (reg));

#define REG_A 0
#define REG_C 1
#define REG_D 2
#define REG_B 3
#define REG_SP 4
#define REG_BP 5
#define REG_SI 6
#define REG_DI 7

namespace x86 {

struct push_t {
	char cmd; // 0x68
	int32_t value; //
}__attribute__ ((__packed__));

struct call_t {
	char cmd; // 0xe8
	int32_t relative_addr; //
}__attribute__ ((__packed__));

struct jump_t {
	char cmd; // 0xe9
	int32_t relative_addr;
}__attribute__ ((__packed__));

struct sub_t {
	char cmd; // 0x81
	char modrm; // ModRM(3, 0, ESP);
	int32_t value; // 0x00000008;
}__attribute__ ((__packed__));

struct push_register_t {
	char cmd;
}__attribute__ ((__packed__));

struct pop_register_t {
	char cmd;
}__attribute__ ((__packed__));

struct mov_esp_to_ebp_t {
	char cmd0;
	char cmd1;
}__attribute__ ((__packed__));

struct leave_t {
	char cmd;
}__attribute__ ((__packed__));

struct ret_t {
	char cmd;
}__attribute__ ((__packed__));

__inline__ void push(push_t & ths, int32_t v) {
	ths.cmd = 0x68;
	ths.value = v;
}

__inline__ void call(call_t & ths, int32_t addr) {
	ths.cmd = 0xe8;
	ths.relative_addr = addr;
}

__inline__ void jump(jump_t & ths, int32_t addr) {
	ths.cmd = 0xe9;
	ths.relative_addr = addr;
}

__inline__ void sub_esp(sub_t & ths, int32_t addr) {
	ths.cmd = 0x81;
	ths.modrm = ModRM(3, 0, REG_SP);
	ths.value = addr;
}

__inline__ void push_register(push_register_t & ths, char reg) {
	ths.cmd = 0x50 + reg;
}

__inline__ void pop_register(pop_register_t & ths, char reg) {
	ths.cmd = 0x58 + reg;
}

__inline__ void mov_esp_to_ebp(mov_esp_to_ebp_t & ths) {
	ths.cmd0 = 0x89;
	ths.cmd1 = 0xe5;
}

__inline__ void leave(leave_t & ths) {
	ths.cmd = 0xc9;
}

__inline__ void ret(ret_t & ths) {
	ths.cmd = 0xc3;
}

}
;
#endif /* X86_STRUCT_HH_ */
