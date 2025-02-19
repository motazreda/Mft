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

#include "general_analysis.h"
#include "pe_analyzer.h"


void PeAnalyzer(unsigned char *buffer, char opt)
{
	IMAGE_DOS_HEADER *dos;
	IMAGE_NT_HEADERS *ntheader;
	IMAGE_IMPORT_DESCRIPTOR *import_desc;
	IMAGE_EXPORT_DIRECTORY *export_desc;

	int n, section_size = 0;
	int data_dir;
	char *data_dir_desc;

	int import_count = 0; //for count imports

	printf("\x1B[34m\t########Begin Analyzing########\n");
	if (opt == 'a'){ // all option to dump all information
	// Dos Header
	dos = (IMAGE_DOS_HEADER *)buffer;
	printf("\te_lfanew: %d\n", dos->e_lfanew);
	// End Dos Header
	// begin Pe Header
	unsigned char *PE_HEADER = (unsigned char *)malloc(sizeof(buffer[dos->e_lfanew])); //allocate
	PE_HEADER = &buffer[dos->e_lfanew];
	ntheader = (IMAGE_NT_HEADERS *)PE_HEADER;
	// now print some useful fields for reverser
	printf("\t\x1B[33m######Information######\n");
	printf("\t\x1B[33mSignature: (hex) %02x (ascii) %s\n", ntheader->Signature, &ntheader->Signature);
	printf("\t\x1B[33mMachine: %04x\n", ntheader->file_header.Machine);
	printf("\t\x1B[33mNumber Of Setions: %d\n", ntheader->file_header.NumberOfSections);
	printf("\t\x1B[33mTimeDateStamp: %d\n", ntheader->file_header.TimeDateStamp);
	printf("\t\x1B[33mAddress Of Entry Point: %4x\n", ntheader->op_header.AddressOfEntryPoint);
	printf("\t\x1B[33mSize Of Image: %4x\n", ntheader->op_header.ImageBase);
	printf("\t\x1B[33m######End Information######\n\n");
	// end it here
	printf("\t\x1B[32m#####Section Header#####\n");
	int g=0;
	int realoffset[ntheader->file_header.NumberOfSections];
	for(n=1;n<=ntheader->file_header.NumberOfSections;n++) {
		IMAGE_SECTION_HEADER *secheader = (IMAGE_SECTION_HEADER *)(PE_HEADER + sizeof(IMAGE_NT_HEADERS) + section_size);
		section_size += sizeof(IMAGE_SECTION_HEADER);
		printf("\t\x1B[32mSection Name: %c%c%c%c%c%c%c%c\n", secheader->Name[0], secheader->Name[1], secheader->Name[2], secheader->Name[3], secheader->Name[4], secheader->Name[5], secheader->Name[6], secheader->Name[7]);
		printf("\t\x1B[32mVirtualSize: %04x\n", secheader->VirtualSize);
		printf("\t\x1B[32mVirtualAddress: %04x\n", secheader->VirtualAddress);
		printf("\t\x1B[32mSizeOfRawData: %04x\n", secheader->SizeOfRawData);
		printf("\t\x1B[32mRaw Data: %04x\n", secheader->PointerToRawData);
		printf("\t\x1B[32mPointerToRelocations: %04x\n", secheader->PointerToRelocations);
		printf("\t\x1B[32mPointerToLinenumbers: %04x\n", secheader->PointerToLinenumbers);
		printf("\t\x1B[32mNumberOfRelocations: %02x\n", secheader->NumberOfRelocations);
		printf("\t\x1B[32mNumberOfLinenumbers: %02x\n", secheader->NumberOfLinenumbers);
		printf("\t\x1B[32mCharacteristics: %04x\n", secheader->Characteristics);
		printf("\t\x1B[32m-------------------------\n");
		realoffset[n] = secheader->VirtualAddress - secheader->PointerToRawData;
		// printf("\nsubstract this from any RVA in this section : %x\n", realoffset[n]);
	}
	printf("\t\x1B[32m#####End Of Sections#####\n\n");
	// End Pe Header

	//Begin Dumping Data Directory
	printf("\t########Data Directories########\n\n");
	// int data_dir
	// char *data_dir_desc
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

		printf("\t\x1B[33m######%s######\n\n", data_dir_desc);
		printf("\t\x1B[33mVirtual Address: %08x\n", ntheader->op_header.DataDirectory[data_dir].VirtualAddress);
		printf("\t\x1B[33mSize: %08x\n\n", ntheader->op_header.DataDirectory[data_dir].Size);
		// begin def variables
		int koko;
		section_size = 0;
		int realaddr;
		int fileoffset;
		int funcAddr;
		int CountFunc = 0;
		char *funcAddrStr = (char *)malloc(sizeof(short));
		int checkOrd=0;
		char *checkOrdStr = (char *)malloc(sizeof(int));
		int test;
		// data declaration for export 
		int export_count = 0 ;
		int count_num_of_func = 0;
		char *ExAddrStr = (char *)malloc(sizeof(int));
		int ExAddr = 0;
		// end it here
		// end def variables 
		for(koko=1;koko<=ntheader->file_header.NumberOfSections;koko++) {
			IMAGE_SECTION_HEADER *secheader = (IMAGE_SECTION_HEADER *)(PE_HEADER + sizeof(IMAGE_NT_HEADERS) + section_size);
			section_size += sizeof(IMAGE_SECTION_HEADER);
			if(ntheader->op_header.DataDirectory[data_dir].VirtualAddress <= (secheader->VirtualAddress + secheader->VirtualSize)
				&& ntheader->op_header.DataDirectory[data_dir].VirtualAddress >= secheader->VirtualAddress)
			{
				fileoffset = (secheader->VirtualAddress - secheader->PointerToRawData); //this the pointer to row data to get the real offset
				realaddr = (ntheader->op_header.DataDirectory[data_dir].VirtualAddress - (secheader->VirtualAddress - secheader->PointerToRawData)); // and this is the real offset of the data directory
				printf("\tOffset: %x\n", realaddr);
				printf("\tthe file offset: %x\n", fileoffset);
				printf("\tSection: %c%c%c%c%c%c%c%c\n", secheader->Name[0], secheader->Name[1], secheader->Name[2], secheader->Name[3], secheader->Name[4], secheader->Name[5], secheader->Name[6], secheader->Name[7]);
			} else {
				continue;
			}
		}
		if(data_dir == 0) {
			if(ntheader->op_header.DataDirectory[data_dir].VirtualAddress != 0) {
				export_desc = (IMAGE_EXPORT_DIRECTORY *)&buffer[realaddr];
				printf("\t######################################################\n");
				printf("\tCharacteristics %08x\n", export_desc->Characteristics);
				printf("\tTimeDateStamp %08x\n", export_desc->TimeDateStamp);
				printf("\tMajorVersion %04x\n", export_desc->MajorVersion);
				printf("\tMinorVersion %04x\n", export_desc->MinorVersion);
				if(isprint(buffer[export_desc->Name]))
					printf("\tName %s\n", &buffer[export_desc->Name]);
				else
					printf("\tName %x\n", export_desc->Name);

				printf("\tBase %08x\n", export_desc->Base);
				printf("\tNumberOfFunctions %08x\n", export_desc->NumberOfFunctions);
				printf("\tNumberOfNames %08x\n", export_desc->NumberOfNames);
				printf("\tAddressOfFunctions %08x\n", export_desc->AddressOfFunctions);
				printf("\tAddressOfNames %08x\n", export_desc->AddressOfNames);
				printf("\tAddressOfNameOrdinals %08x\n", export_desc->AddressOfNameOrdinals);
				printf("\t######################################################\n");
				
				printf("\t#########################\n");
				printf("\t# Func RVA      Name    #\n");
				printf("\t#########################\n");

				while(1) {
					if(count_num_of_func == export_desc->NumberOfFunctions)
						break;
					// printf("\tfunc RVA %02x%02x\n", buffer[export_desc->AddressOfFunctions + export_count + 1], buffer[export_desc->AddressOfFunctions + export_count]);
					sprintf(ExAddrStr, "%02x%02x%02x%02x", buffer[export_desc->AddressOfNames  - fileoffset + export_count + 3], buffer[export_desc->AddressOfNames - fileoffset + export_count + 2], buffer[export_desc->AddressOfNames - fileoffset + export_count + 1], buffer[export_desc->AddressOfNames - fileoffset + export_count]);	
					sscanf(ExAddrStr, "%08x", &ExAddr);
					// printf("\n%x\n", ExAddr);
					// printf("\t##########Name###########\n");
					printf("\t  %02x%02x\t\t%s\n", buffer[export_desc->AddressOfFunctions - fileoffset + export_count + 1], buffer[export_desc->AddressOfFunctions - fileoffset + export_count], &buffer[ExAddr - fileoffset]);
					// printf("\t############################\n");
					export_count += 4;
					count_num_of_func++;
				}
				printf("\t############################\n");	
			} else {
				printf("\tNo Exports\n");
			}		
		// now focus on Import Descriptor Table
		} else if(data_dir == 1) { // if it Import Directory
			while(1) {
				import_desc = (IMAGE_IMPORT_DESCRIPTOR *)&buffer[realaddr + import_count];
				if(import_desc->Characteristics == 0) {
					break;
				}
				// here we check if this module's function is ordinal or not
				// here we comment this fuckin line its drive me crazy realyyyy :(
				sprintf(checkOrdStr, "%02x%02x%02x%02x", buffer[import_desc->OriginFirstThunk.Ordinal+3-fileoffset], buffer[import_desc->OriginFirstThunk.Ordinal+2-fileoffset], buffer[import_desc->OriginFirstThunk.Ordinal+1-fileoffset], buffer[import_desc->OriginFirstThunk.Ordinal-fileoffset]);
				sscanf(checkOrdStr, "%08x", &checkOrd);
				// checkOrd = strtol(checkOrdStr, NULL, 16);
				// end of fuckin line :S

				// this two line below for modue check if it is function name or ordinal
			 	printf("\t####################\n");
				printf("\tImport Name Table: %08x\n", import_desc->OriginFirstThunk.Function);
			 	printf("\tTimeDateStamp: %x\n", import_desc->TimeDateStamp);
			 	printf("\tForwarderChain: %x\n", import_desc->ForwarderChain);
			 	printf("\tName: %s\n", &buffer[import_desc->Name - fileoffset]);
			 	printf("\tImport Address Table: %x\n", import_desc->FirstThunk.Function - fileoffset);
			 	printf("\t####################\n");
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
			 			if(buffer[import_desc->OriginFirstThunk.Function - fileoffset + CountFunc + 1] == 0x00 && buffer[import_desc->OriginFirstThunk.Function - fileoffset + CountFunc] == 0x00){
			 				break;
			 			}
			 			sprintf(funcAddrStr, "%02x%02x%02x%02x", buffer[import_desc->OriginFirstThunk.Function - fileoffset + CountFunc + 3], buffer[import_desc->OriginFirstThunk.Function - fileoffset + CountFunc + 2], buffer[import_desc->OriginFirstThunk.Function - fileoffset + CountFunc + 1], buffer[import_desc->OriginFirstThunk.Function - fileoffset + CountFunc]);
			 			// funcAddr = strtol(funcAddrStr, NULL, 16);
			 			sscanf(funcAddrStr, "%08x", &funcAddr);
			 			// check if its ordinal or not
			 			if(funcAddr & IMAGE_ORDINAL_FLAG32) // its ordinal not a function
			 				printf("\t%08x\n", funcAddr);
			 			else
			 				printf("\t%02x%02x\t\t%s\n", buffer[funcAddr-fileoffset+1], buffer[funcAddr-fileoffset], &buffer[funcAddr-fileoffset+2]);
			 			// end of check here
			 			funcAddr=0;
			 			CountFunc+=4;
			 		}
			 		CountFunc=0;
			 	printf("\t############################\n");
			 	printf("\n\n");
				import_count += sizeof(IMAGE_IMPORT_DESCRIPTOR);
			}
			
		}
		// end of Import Descriptor Table
	}
}
}
