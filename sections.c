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
 
#include "sections.h"
#include "pe_analyzer.h"

 
void ListSections(unsigned char *buffer, char opt)
{
	IMAGE_DOS_HEADER *dos;
	IMAGE_NT_HEADERS *ntheader;

	int n, section_size = 0;
	int data_dir;
	char *data_dir_desc;

	int import_count = 0; //for count imports

	printf("\t########Begin Analyzing########\n");
	// Dos Header
	dos = (IMAGE_DOS_HEADER *)buffer;
	// End Dos Header
	// begin Pe Header
	unsigned char *PE_HEADER = (unsigned char *)malloc(sizeof(buffer[dos->e_lfanew])); //allocate
	PE_HEADER = &buffer[dos->e_lfanew];
	ntheader = (IMAGE_NT_HEADERS *)PE_HEADER;
	printf("\t#####Section Header#####\n");
	int g=0;
	int realoffset[ntheader->file_header.NumberOfSections];
	for(n=1;n<=ntheader->file_header.NumberOfSections;n++) {
		IMAGE_SECTION_HEADER *secheader = (IMAGE_SECTION_HEADER *)(PE_HEADER + sizeof(IMAGE_NT_HEADERS) + section_size);
		section_size += sizeof(IMAGE_SECTION_HEADER);
		printf("\tSection Name: %c%c%c%c%c%c%c%c\n", secheader->Name[0], secheader->Name[1], secheader->Name[2], secheader->Name[3], secheader->Name[4], secheader->Name[5], secheader->Name[6], secheader->Name[7]);
		printf("\tVirtualSize: %04x\n", secheader->VirtualSize);
		printf("\tVirtualAddress: %04x\n", secheader->VirtualAddress);
		printf("\tSizeOfRawData: %04x\n", secheader->SizeOfRawData);
		printf("\tRaw Data: %04x\n", secheader->PointerToRawData);
		printf("\tPointerToRelocations: %04x\n", secheader->PointerToRelocations);
		printf("\tPointerToLinenumbers: %04x\n", secheader->PointerToLinenumbers);
		printf("\tNumberOfRelocations: %02x\n", secheader->NumberOfRelocations);
		printf("\tNumberOfLinenumbers: %02x\n", secheader->NumberOfLinenumbers);
		printf("\tCharacteristics: %04x\n", secheader->Characteristics);
		printf("\t-------------------------\n");
		realoffset[n] = secheader->VirtualAddress - secheader->PointerToRawData;
		// printf("\nsubstract this from any RVA in this section : %x\n", realoffset[n]);
	}
	printf("\t#####End Of Sections#####\n\n");
	// End Pe Header
}

