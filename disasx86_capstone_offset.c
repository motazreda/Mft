/*
 *
 *  Copyright (C) 2014-2015 Motaz Reda <motazkhodair@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to motaz reda 
 *  motazkhodair@gmail.com
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <capstone/capstone.h>

#include "disasx86_capstone_offset.h"
#include "pe_analyzer.h"



int DisassembleCapOffset(unsigned char *buffer, char opt, unsigned char *second_option, int file_len, unsigned char *third_option)
{
	// this option (third option) is responsible for disassemble where it find what and stop
	unsigned char *length = third_option;
	int offset = 0;
	int offset_file = 0;
	int buf_len=0;
	int offset_address = 0;
	// some unit testing ;)
	int offsets = strtoul(second_option, NULL, 16);
	offset = RealFileOffset(buffer, offsets);
	offset_address = offset;
	//printf("the real offset is %x\n", offset);
	printf("file len is %d\n", file_len);
	unsigned char *buf = (unsigned char *)malloc(file_len-offset);
	for(offset_file=0 ; offset_file+offset != file_len ; offset_file++) {
		buf[offset_file] = buffer[offset+offset_file];
		buf_len++;
	}
	
	csh handle;
	
	cs_insn *insn;
	
	size_t count;
	
	if(cs_open(CS_ARCH_X86, CS_MODE_32, &handle) != CS_ERR_OK)
		return -1;

	cs_option(handle, CS_OPT_SKIPDATA, CS_OPT_ON);
	count = cs_disasm(handle, buf, buf_len, offset, 0, &insn);
	if(count > 0) {
		size_t j;
		
		for(j=0;j<count;j++){
			if (strcmp(length, insn[j].mnemonic)==0 || strcmp("retf", insn[j].mnemonic) == 0) {
				printf("\x1B[35m\t[ 0x%08x ] : \t\x1B[32m%s\t%s\n", offset_address, insn[j].mnemonic, insn[j].op_str);
				break;
			}
			printf("\x1B[35m\t[ 0x%08x ] : \t\x1B[32m%s\t%s\n", offset_address, insn[j].mnemonic, insn[j].op_str);
			offset_address += insn[j].size;
		}
		cs_free(insn, count);
	} else {
			printf("failed disassemble\n");
	}
}
