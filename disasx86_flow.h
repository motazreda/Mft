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

char * HexToAsciiFlow(char *ins, int len){
	int i, n;
	char *addrstr = malloc(len);
	int addrint;
	char *addr_sum_str = malloc(len); 
	// snprintf(addrstr, "")
	for(i=0; i <= len-1;i+=2){	
		printf("\n###################\n%c%c\n###################\n", ins[i], ins[i+1]);
		if(ins[i] == '0' && ins[i+1] == 'x')
			printf("");
		else{
			snprintf(addrstr, 3, "%c%c", ins[i], ins[i+1]);
			sscanf(addrstr, "%x", &addrint);
			if(isascii(addrint))
				printf(" ASCII %c\n", addrint);
			strncat(addr_sum_str, &ins[i], 2);
		}
	}
	printf("\nADDRESS: %s\n", addr_sum_str);
	return ins;
}

// this function is very simple cit takes pointer to insn->ascii and split it with ',' delimiter and
// and return the hex data of it example "0x90283819" something like that
char *FilterFlow(char *ins){
	char *printable;
	char *hex;
	printable = strtok(ins, ",");
	while(printable != NULL){
		if(strstr(printable, "0x") != NULL && strstr(printable, "[") == NULL)
			hex = printable;
		printable = strtok(NULL, ",");
	}
	return hex;
}

char *FilterJmpFlow(char *ins){
	char *printable;
	char *hex;
	printable = strtok(ins, "    ");
	while(printable != NULL){
		if(strstr(printable, "0x") != NULL && strstr(printable, "[") == NULL)
			hex = printable;
		printable = strtok(NULL, ",");
	}
	return hex;
}

char *ExtractJmpAddrFlow(char *jmpins) {
	char *jmpaddr;
	char *hexaddr;
	jmpaddr = strtok(jmpins, "    ");
	while(jmpaddr != NULL){
		if(strstr(jmpaddr, "0x") != NULL && strstr(jmpaddr, "[") == NULL)
			hexaddr = jmpaddr;
		else
			hexaddr = NULL;
		jmpaddr = strtok(NULL, "    "); 
	}
	return hexaddr;
}


void Recurse(char *j) {
	int Addr = 0;
	char *part;
	Addr = strtoul(j, &part, 16);
	printf("the jmp address is %08x\n", Addr);
}

static void display_insn_flow(const opdis_insn_t *insn, void *arg){
	PrintToFile(insn->ascii);
	char *addr_to_hex;
	char *instru = (char *)malloc(strlen(insn->ascii) + 1);
	strncpy(instru, insn->ascii, strlen(insn->ascii) + 1);
	char *word[9] = {"jmp", "je", "call", "ja", "jge", "jle", "jg", "jl", "jne"};
	if(strstr(insn->ascii, "0x") != NULL && \
		strstr(insn->ascii, "[") == NULL && \
			strstr(insn->ascii, "jmp") == NULL && \
			strstr(insn->ascii, "call") == NULL && \
			strstr(insn->ascii, "ja") == NULL && \
			strstr(insn->ascii, "je") == NULL && \
			strstr(insn->ascii, "jge") == NULL && \
			strstr(insn->ascii, "jle") == NULL && \
			strstr(insn->ascii, "jg") == NULL && \
			strstr(insn->ascii, "jl") == NULL && \
			strstr(insn->ascii, "jne") == NULL && \
			strstr(insn->ascii, "ds:") == NULL && \
			strstr(insn->ascii, "ss:") == NULL && \
			strstr(insn->ascii, "fs:") == NULL && \
			strstr(insn->ascii, "gs")  == NULL && \
			strstr(insn->ascii, "jbe") == NULL && \
			strstr(insn->ascii, ",") != NULL) {
		
		// printable = strtok(insn->ascii, ", ");
		// while(printable != NULL){
		// 	if(strstr(printable, "0x") != NULL){
		// 		hex = printable;
		// 		// printf("\t%08x\t%s\n", insn->offset, insn->ascii);
		// 		// printf("# %s", printable);
		// 	}
		// 	printable = strtok(NULL, ",");
		// 	// printf("%08x\t%s\n", insn->offset, insn->ascii);
		// }
		char *ins = FilterFlow(insn->ascii);
		// addr_to_hex = HexToAscii(ins, strlen(ins));
		// printf("\n#######\n%s\n#######", addr_to_hex);

		// printf("\t%s\n", ins);
		printf("\t%08x\t%s\t\t# HEX %s\n", insn->offset, instru, ins);
	}
	else if (strstr(insn->ascii, word[0]) != NULL && \
			strstr(insn->ascii, "0x") != NULL){
		char *insn_ascii_backup = (char *)malloc(strlen(insn->ascii)+1);
		memcpy(insn_ascii_backup, insn->ascii, strlen(insn->ascii));
		char *jmphexaddr;
		jmphexaddr = ExtractJmpAddrFlow(insn->ascii);
		printf("\t%08x\t%s\t\t# JMP ADDR %s\n", insn->offset, insn_ascii_backup, jmphexaddr);
		if(jmphexaddr != NULL)
			Recurse(jmphexaddr);
		jmphexaddr = NULL;
		insn_ascii_backup = NULL;

	}
	// else if(strstr(insn->ascii, word[0]) != NULL)
		// printf("\t%08x\t%s\t\t# JMP HERE\n", insn->offset, insn->ascii);
	else if (strstr(insn->ascii, word[1]) != NULL)
		printf("\t%08x\t%s\t\t# JE HERE\n", insn->offset, insn->ascii);
	else if (strstr(insn->ascii, word[2]) != NULL)
		printf("\t%08x\t%s\t\t# CALL HERE\n", insn->offset, insn->ascii);
	else if (strstr(insn->ascii, word[3]) != NULL)
		printf("\t%08x\t%s\t\t# JA HERE\n", insn->offset, insn->ascii);
	else if (strstr(insn->ascii, word[4]) != NULL)
		printf("\t%08x\t%s\t\t# JGE HERE\n", insn->offset, insn->ascii);
	else if (strstr(insn->ascii, word[5]) != NULL)
		printf("\t%08x\t%s\t\t# JLE HERE\n", insn->offset, insn->ascii);
	else if (strstr(insn->ascii, word[6]) != NULL)
		printf("\t%08x\t%s\t\t# JG HERE\n", insn->offset, insn->ascii);
	else if (strstr(insn->ascii, word[7]) != NULL)
		printf("\t%08x\t%s\t\t# JL HERE\n", insn->offset, insn->ascii);
	else if (strstr(insn->ascii, word[8]) != NULL)
		printf("\t%08x\t%s\t\t# JNE HERE\n", insn->offset, insn->ascii);
	else	
		printf("\t%08x\t%s\n", insn->offset, insn->ascii);

// here we want to make jmps if jumps detected 
	// char *instru_jmp = (char *)malloc(strlen(insn->ascii)+1);
	// strncat(instru_jmp, insn->ascii, strlen(insn->ascii));
	// if(strstr(insn->ascii, "jmp") != NULL) {
	// 	char *hex_data;
	// 	hex_data = FilterJmpFlow(insn->ascii);
	// 	printf("\t\t\t#JMP ADDRESS  %s\n", 	hex_data);
	// }	
// end jmp detector here

}

void DisassembleFlow(unsigned char *buffer, opdis_buf_t buffer2, void *name, char opt){
	IMAGE_DOS_HEADER *dos;
	IMAGE_NT_HEADERS *ntheader;
	IMAGE_IMPORT_DESCRIPTOR *import_desc;
	PIMAGE_THUNK_DATA *names;

	int n, section_size = 0;
	int data_dir;
	char *data_dir_desc;

	opdis_off_t offset = 0;
	opdis_off_t length = 0;
	opdis_t o;
	int rv;
	o = opdis_init();
	opdis_set_arch(o, bfd_arch_i386, bfd_mach_i386_i386_intel_syntax, (disassembler_ftype)fprintf);
	opdis_set_x86_syntax(o, opdis_x86_syntax_intel);
	CheckFileExists();
	opdis_set_display(o, display_insn_flow, (void *)name);

	
	// ud_t ud_obj;
	unsigned char *buf;
	int import_count = 0; //for count imports

	printf("\t########Disassembling########\n");
	// Dos Header
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
			offset = (opdis_off_t)(ntheader->op_header.AddressOfEntryPoint - fileoffset);
		}
	}
	
	// offset = (opdis_off_t)ntheader->op_header.AddressOfEntryPoint;
	printf("\tAddress Of Entry Point is %04x\n", offset);
	length = (opdis_off_t)ntheader->op_header.BaseOfCode;
	printf("\tBase Of Code: %04x\n", length);
	int gg;
	buf = (unsigned char *)malloc(length);
	for(gg=0;gg!=length;gg++){
		buf[gg] = buffer[offset+gg];
		// printf("%c", buf[gg]);
	}
	printf("\t-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#\n");
	rv = opdis_disasm_linear(o, buffer2, offset, (offset+length));
	opdis_term(o);
	printf("\t-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#\n");
	printf("\t########End Disassembling########\n\n");
	// End Pe Header
	free(buf);
}
