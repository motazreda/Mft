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

#include "disasx86_capstone.h"
#include "pe_analyzer.h"



int DisassembleCap(unsigned char *buffer, char opt, unsigned char *filename, int file_len)
{
	printf("file name is : %s\n", filename);
	printf("buffer is %s\n", buffer);
	printf("option is %c\n", opt);

	// here begin for getting the real entry point for every PE file 
	IMAGE_DOS_HEADER *dos;
	IMAGE_NT_HEADERS *ntheader;

	int n, section_size = 0;
	int data_dir;
	char *data_dir_desc;
	int import_count = 0; 
	int offset = 0;
	int length = 0;

	dos = (IMAGE_DOS_HEADER *)buffer;
	// End Dos Header
	// begin Pe Header
	unsigned char *PE_HEADER = (unsigned char *)malloc(sizeof(buffer[dos->e_lfanew]) * sizeof(unsigned char *) + 1); //allocate
	PE_HEADER = &buffer[dos->e_lfanew];
	ntheader = (IMAGE_NT_HEADERS *)PE_HEADER; 

	int g=0;
	int realoffset=0;
	int fileoffset = 0;
	for(n=1;n<=ntheader->file_header.NumberOfSections;n++) {
		IMAGE_SECTION_HEADER *secheader = (IMAGE_SECTION_HEADER *)(PE_HEADER + sizeof(IMAGE_NT_HEADERS) + section_size);
		section_size += sizeof(IMAGE_SECTION_HEADER);
		// here we check for the address are its in a section or not 
		if(ntheader->op_header.AddressOfEntryPoint <= (secheader->VirtualAddress + secheader->VirtualSize) && 
			ntheader->op_header.AddressOfEntryPoint >= secheader->VirtualAddress) {
			// realoffset = secheader->VirtualAddress - secheader->PointerToRawData;


			fileoffset = (secheader->VirtualAddress - secheader->PointerToRawData); //this the pointer to row data to get the real offset
			// realoffset = (ntheader->op_header.DataDirectory[data_dir].VirtualAddress - (secheader->VirtualAddress - secheader->PointerToRawData)); // and this is the real offset of the data directory
			printf("\treal offset in disk is: %x for section %c%c%c%c%c%c%c%c\n", fileoffset, secheader->Name[0], secheader->Name[1], secheader->Name[2], secheader->Name[3], secheader->Name[4], secheader->Name[5], secheader->Name[6], secheader->Name[7]);
			offset = (ntheader->op_header.AddressOfEntryPoint - fileoffset);
			printf("the offset is %x\n", offset);
		}
	}

	length = ntheader->op_header.BaseOfCode;
	int offset_file = 0;
	int buf_len = 0;
	unsigned char *buf = (unsigned char *)malloc(file_len - offset);
	for(offset_file=0 ; offset_file+offset!=file_len ; offset_file++) {
		buf[offset_file] = buffer[offset+offset_file];
		buf_len++;
	} 
	// here end of getting the entry point 
	csh handle;
	cs_insn *insn;
	size_t count;
	int offsets = offset+fileoffset;
	printf("buffer size is %d\n", file_len);
	// printf("buf len is %d\n", buf_len);
	if(cs_open(CS_ARCH_X86, CS_MODE_32, &handle) != CS_ERR_OK)
		return -1;

	cs_option(handle, CS_OPT_SKIPDATA, CS_OPT_ON);
	cs_option(handle, CS_OPT_DETAIL, CS_OPT_ON);
	count = cs_disasm(handle, buf, length, offset+fileoffset, 0, &insn);
	if(count > 0) {
		size_t j;
		int n;

		for(j=0;j<count;j++) {
			cs_insn *i = &(insn[j]);

			if(strstr(insn[j].mnemonic, "jmp") != NULL){
				// print jmp instruction in white color
				printf("\x1B[37m\t[ 0x%08x ] : \t\x1B[37m%s\t\t%s\n", offsets, i->mnemonic, i->op_str);	
			}else if(strstr(i->mnemonic, "call") != NULL){
				int instruction;
				unsigned char *API;
				cs_detail *detail = i->detail;
				for(n=0;n<detail->regs_read_count;n++){
					cs_x86_op *op = &(detail->x86.operands[n]);
					switch(op->type){
						case X86_OP_MEM:
							printf("index is %x\n", op->mem.disp);
							instruction = op->mem.disp;
							break;
						default:
							printf("index is %x\n", op->reg);
							instruction = op->reg;
							break;
					}
					//API = GetApi(instruction, buffer);
					// printf("Api Name is %s\n", API);

					
				}
				printf("---->\x1B[35m\t[ 0x%08x ] : \t\x1B[32m%s\t\t%s\n", offsets, i->mnemonic, i->op_str);
				// free(op_bak);
			}else{
				// print another instructions except jmp with green
				printf("\x1B[35m\t[ 0x%08x ] : \t\x1B[32m%s\t\t%s\n", offsets, i->mnemonic, i->op_str);				
			}
		
			offsets += insn[j].size; // i decide to makeit manually :D
		}
		cs_free(insn, count);
	}else{
		printf("failed disassemble\n");

	}

	cs_close(&handle);
	return 0;
}
