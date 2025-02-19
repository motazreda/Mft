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

#include "imports.h"
#include "pe_analyzer.h"



void DumpImports(unsigned char *buffer, char opt)
{
	IMAGE_DOS_HEADER *dos;
	IMAGE_NT_HEADERS *ntheader;
	IMAGE_IMPORT_DESCRIPTOR *import_desc;

	int n, section_size = 0;
	int data_dir;
	char *data_dir_desc;

	int import_count = 0; //for count imports

	printf("\t\x1B[33m########Dumping Imports########\n");
	// Dos Header
	dos = (IMAGE_DOS_HEADER *)buffer;
	// End Dos Header
	// begin Pe Header
	unsigned char *PE_HEADER = (unsigned char *)malloc(sizeof(buffer[dos->e_lfanew])); //allocate
	PE_HEADER = &buffer[dos->e_lfanew];
	ntheader = (IMAGE_NT_HEADERS *)PE_HEADER;
	
	int g=0;
	// End Pe Header

	//Begin Dumping Data Directory
	
	for(data_dir=0;data_dir<IMAGE_NUMBEROF_DIRECTORY_ENTRIES;data_dir++) {
		switch(data_dir) {
			case 0:
				data_dir_desc = "EXPORT Entry";
				break;
			case 1:
				data_dir_desc = "IMPORT Entry";
				break;
			case 2:
				data_dir_desc = "RESOURCE Directory";
				break;
			case 3:
				data_dir_desc = "EXCEPTION Entry";
				break;
			case 4:
				data_dir_desc = "SECURITY Entry";
				break;
			case 5:
				data_dir_desc = "BASERELOC Entry";
				break;
			case 6:
				data_dir_desc = "DEBUG Entry";
				break;
			case 7:
				data_dir_desc = "COPYRIGHT Entry";
				break;
			case 8:
				data_dir_desc = "GLOBALPTR Entry";
				break;
			case 9:
				data_dir_desc = "TLS Entry";
				break;
			case 10:
				data_dir_desc = "LOAD_CONFIG Entry";
				break;
			case 11:
				data_dir_desc = "BOUNT IMPORT Entry";
				break;
			case 12:
				data_dir_desc = "IAT Entry";
				break;
			case 13:
				data_dir_desc = "DELAY IMPORT Entry";
				break;
			case 14:
				data_dir_desc = "COM Descriptor Entry";
				break;
			case 15:
				data_dir_desc = ".NET MetaData";
				break;
			default:
				data_dir_desc = "Nothing bigger than 15 data direcotry";
				break;
		}

		// printf("\t######%s######\n\n", data_dir_desc);
		// printf("\tVirtual Address: %08x\n", ntheader->op_header.DataDirectory[data_dir].VirtualAddress);
		// printf("\tSize: %08x\n\n", ntheader->op_header.DataDirectory[data_dir].Size);
		// begin def variables
		int koko;
		section_size = 0;
		int realaddr;
		int fileoffset;
		int funcAddr;
		int CountFunc = 0;
		char *funcAddrStr = (char *)malloc(sizeof(short));
		int checkOrd;
		char *checkOrdStr = (char *)malloc(sizeof(int));
		// end def variables 
		for(koko=1;koko<=ntheader->file_header.NumberOfSections;koko++) {
			IMAGE_SECTION_HEADER *secheader = (IMAGE_SECTION_HEADER *)(PE_HEADER + sizeof(IMAGE_NT_HEADERS) + section_size);
			section_size += sizeof(IMAGE_SECTION_HEADER);
			if(ntheader->op_header.DataDirectory[data_dir].VirtualAddress <= (secheader->VirtualAddress + secheader->VirtualSize)
				&& ntheader->op_header.DataDirectory[data_dir].VirtualAddress >= secheader->VirtualAddress)
			{
				fileoffset = (secheader->VirtualAddress - secheader->PointerToRawData); //this the pointer to row data to get the real offset
				realaddr = (ntheader->op_header.DataDirectory[data_dir].VirtualAddress - (secheader->VirtualAddress - secheader->PointerToRawData)); // and this is the real offset of the data directory
			} else {
				continue;
			}
		}
		// now focus on Import Descriptor Table
		// if(opt == 'i'){ // if import option choosed
		if(data_dir == 1) { // if it Import Directory
			// printf("IMPORT Address: %08x\n", ntheader->op_header.DataDirectory[data_dir].VirtualAddress);
			while(1){
				import_desc = (IMAGE_IMPORT_DESCRIPTOR *)&buffer[realaddr + import_count];
				if(import_desc->Characteristics == 0) {
					break;
				}
				// here we check if this module's function is ordinal or not
				sprintf(checkOrdStr, "%02x%02x%02x%02x", buffer[import_desc->OriginFirstThunk.Ordinal+3 - fileoffset], buffer[import_desc->OriginFirstThunk.Ordinal+2 - fileoffset], buffer[import_desc->OriginFirstThunk.Ordinal+1 - fileoffset], buffer[import_desc->OriginFirstThunk.Ordinal - fileoffset]);
				sscanf(checkOrdStr, "%08x", &checkOrd);
				// this two line below for modue check if it is function name or ordinal
			 	printf("\t#########################################################\n");
				printf("\t# Import Name Table: %08x\n", import_desc->OriginFirstThunk.Function);
			 	printf("\t# TimeDateStamp: %x\n", import_desc->TimeDateStamp);
			 	printf("\t# ForwarderChain: %x\n", import_desc->ForwarderChain);
			 	printf("\t# Name: %s\n", &buffer[import_desc->Name - fileoffset]);
			 	printf("\t# Import Address Table: %x\n", import_desc->FirstThunk.Function - fileoffset);
			 	printf("\t#########################################################\n");

			 	if(checkOrd & IMAGE_ORDINAL_FLAG32) {
					printf("\t##########Ordinal###########\n");
					printf("\t############################\n");
					printf("\t#         Ordinal Number   #\n");
					printf("\t############################\n");
				} else {
			 		printf("\t##########Function##########\n");
			 		printf("\t######################################\n");
					printf("\t# Hint\t\tFunction Name        #\n");
					printf("\t######################################\n");
			 	}
			 		while(1) {
			 			if(buffer[import_desc->OriginFirstThunk.Function - fileoffset + CountFunc + 1] == 0x00 && buffer[import_desc->OriginFirstThunk.Function - fileoffset + CountFunc] == 0x00) {
			 				break;
			 			}
			 			sprintf(funcAddrStr, "%02x%02x%02x%02x", buffer[import_desc->OriginFirstThunk.Function - fileoffset + CountFunc + 3], buffer[import_desc->OriginFirstThunk.Function - fileoffset + CountFunc + 2], buffer[import_desc->OriginFirstThunk.Function - fileoffset + CountFunc + 1], buffer[import_desc->OriginFirstThunk.Function - fileoffset + CountFunc]);
			 			sscanf(funcAddrStr, "%08x", &funcAddr);
			 			// check if its ordinal or not
			 			if(funcAddr & IMAGE_ORDINAL_FLAG32) // its ordinal not a function
			 				printf("\t%08x\n", funcAddr);
			 			else
			 				printf("\t%02x%02x\t\t%s\n", buffer[funcAddr-fileoffset+1], buffer[funcAddr-fileoffset], &buffer[funcAddr-fileoffset+2]);

			 				// printf("\t%s\n", &buffer[funcAddr-fileoffset+2]);
			 			// end of check here
			 			funcAddr=0;
			 			CountFunc+=4;
			 		}
			 		CountFunc=0;
			 	printf("\t#####################################################\n");
			 	printf("\n\n");
				import_count += sizeof(IMAGE_IMPORT_DESCRIPTOR);
			}
		}
		// end of Import Descriptor Table
	}		
	//End Dumping Data Directory
}
