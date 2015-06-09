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

 
int DisassembleCapOffset(unsigned char *buffer, char opt, unsigned char *second_option, int file_len, unsigned char *third_option){

	unsigned char *length = third_option;
	int offset = 0;
	int offset_file = 0;
	int buf_len=0;
	// some unit testing ;)
	int offsets = strtoul(second_option, NULL, 16);
	offset = RealFileOffset(buffer, offsets);
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
			printf("\t%s\t%s\n", insn[j].mnemonic, insn[j].op_str);
		}
		cs_free(insn, count);
	}else{
			printf("failed disassemble\n");
	}

	

}
