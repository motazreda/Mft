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
#include "pe_analyzer.h"


#define DEFAULT "no"
#define DEFAULT2 1
#define DEFAULT3 "no"
#define REV(X)      ((X << 24) | (((X>>16)<<24)>>16) | (((X<<16)>>24)<<16) | (X>>24))

void Usage(char *filename)
{
	printf("\n\x1B[33m#############################################\n");
	printf("\x1B[33m# Malware Fragmentation Tool 0.1            #\n");
	printf("\x1B[33m# Author : Motaz Reda                       #\n");
	printf("\x1B[33m# Email: motazkhodair(at)gmail(dot)com      #\n");
	printf("\x1B[33m#############################################\n");
	printf("\x1B[33mUsage: \n");
	printf("\x1B[33m\t-a <filename> for display all Information analyzed\n");
	printf("\x1B[33m\t-I <filename> for display import table information\n");
	printf("\x1B[33m\t-n <section name> <filename> for display section information\n");
	printf("\x1B[33m\t-N <section name> <filename> for display hex dump for section choosed information\n");
	printf("\x1B[33m\t-s <filename> Dump all sections information\n");
	printf("\x1B[33m\t-S <section name> <filename> dump Strings for choosed secion \n");
	printf("\x1B[33m\t-f <filename> Disassemble Flow Oriented from default Entry Point\n");
	printf("\x1B[33m\t-c <offset> <length> <filename> Disassemble for specifiec offset\n");
	exit(0);
}


void GetSection(unsigned char *buffer, char opt, unsigned char *sectionname)
{
	IMAGE_DOS_HEADER *dos;
	IMAGE_NT_HEADERS *ntheader;

	int n, section_size = 0;
	int data_dir;
	char *data_dir_desc;
	unsigned char *section = (unsigned char *)malloc(8);
	int import_count = 0; //for count imports

	printf("\t########Begin Analyzing########\n");
	// Dos Header
	dos = (IMAGE_DOS_HEADER *)buffer;
	// End Dos Header
	// begin Pe Header
	unsigned char *PE_HEADER = (unsigned char *)malloc(sizeof(buffer[dos->e_lfanew])); //allocate
	PE_HEADER = &buffer[dos->e_lfanew];
	ntheader = (IMAGE_NT_HEADERS *)PE_HEADER;
	// printf("\t#####Section Header#####\n");
	int g=0;
	int result_section;
	int realoffset[ntheader->file_header.NumberOfSections];
	for(n=1;n<=ntheader->file_header.NumberOfSections;n++) {
		IMAGE_SECTION_HEADER *secheader = (IMAGE_SECTION_HEADER *)(PE_HEADER + sizeof(IMAGE_NT_HEADERS) + section_size);
		section_size += sizeof(IMAGE_SECTION_HEADER);
		sprintf(section, "%c%c%c%c%c%c%c%c", secheader->Name[0], secheader->Name[1], secheader->Name[2], secheader->Name[3], secheader->Name[4], secheader->Name[5], secheader->Name[6], secheader->Name[7]);
		if(strcmp(sectionname, section) == 0) {
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
		}else if(strcmp(sectionname, section) != 0 && n < ntheader->file_header.NumberOfSections){
			continue;
		}else{
			printf("\tSection Not Found : %s (use -s option for listing all sections)\n");
			break;
		}

		// printf("\nsubstract this from any RVA in this section : %x\n", realoffset[n]);
	}
	// printf("\t#####End Of Sections#####\n\n");
	// End Pe Header
	printf("Got section name : %s\n", sectionname);
}


void CheckFormat(unsigned char *buffer, char opt, unsigned char *extraopt, int extraopt2, unsigned char *extraopt3)
{
	// here we check file format we will begin with PE and ELF
		void *name;
		if(buffer[0] == 0x7f && buffer[1] == 0x45){
			printf("this file is ELF \n");
		}else if(buffer[0] == 0x4d && buffer[1] == 0x5a){
			printf("this file is PE file\n");
			if(opt == 'a')
				PeAnalyzer(buffer, opt);
			else if(opt == 'I')
				DumpImports(buffer, opt);
			else if(opt == 's')
				ListSections(buffer, opt);
			else if(opt == 'n')
				GetSection(buffer, opt, extraopt);
			else if(opt == 'N')
				DumpSecHex(buffer, opt, extraopt);
			else if(opt == 'S')
				DumpStrings(buffer, opt, extraopt);
			else if(opt == 'f')
				DisassembleCap(buffer, opt, extraopt, extraopt2);
			else if(opt == 'c')
				DisassembleCapOffset(buffer, opt, extraopt, extraopt2, extraopt3);
		}
}


int main(int argc, char **argv)
{
	if(argc < 2){
		Usage(argv[0]);
		return(0);
	}
	// usage begin
	int option = 0;
	char optgot;
	// usage end
	int i, file_len;
	FILE *f;
	unsigned char *buffer;
	
	while ((option = getopt(argc, argv, "h:a:I:s:n:N:S:f:c:")) != -1){
		switch(option){
			case 'a':
				printf("file name inserted is %s\n", optarg);
				f = fopen(optarg, "rb");
				// now printing file size
				fseek(f, 0, SEEK_END);
				file_len = ftell(f);
				fseek(f, 0, SEEK_SET);
				printf("file size: %d KB\n", file_len / 1024);
				buffer = (unsigned char *)malloc(file_len); //allocate
				// now time to read the contents of the file
				fread(buffer, file_len, 1, f);
				CheckFormat(buffer, 'a', DEFAULT, DEFAULT2, DEFAULT3);
				free(buffer);
				fclose(f);
				break;
			case 'I':
				printf("file name inserted is %s\n", optarg);
				f = fopen(optarg, "rb");
				// now printing file size
				fseek(f, 0, SEEK_END);
				file_len = ftell(f);
				fseek(f, 0, SEEK_SET);
				printf("file size: %d bytes\n", file_len);
				buffer = (unsigned char *)malloc(file_len); //allocate
				// now time to read the contents of the file
				fread(buffer, file_len, 1, f);
				CheckFormat(buffer, 'I', DEFAULT, DEFAULT2, DEFAULT3);
				fclose(f);
				break;
			case 's':
				printf("file name inserted is %s\n", optarg);
				f = fopen(optarg, "rb");
				// now printing file size
				fseek(f, 0, SEEK_END);
				file_len = ftell(f);
				fseek(f, 0, SEEK_SET);
				printf("file size: %d bytes\n", file_len);
				buffer = (unsigned char *)malloc(file_len); //allocate
				// now time to read the contents of the file
				fread(buffer, file_len, 1, f);
				CheckFormat(buffer, 's', DEFAULT, DEFAULT2, DEFAULT3);
				fclose(f);
				break;
			case 'n':
				if(argc < 4){
					printf("option not completed -n <section name> <filename>\n");
					exit(-1);
				}
				printf("file name inserted is %s\n", argv[3]);
				f = fopen(argv[3], "rb");
				// now printing file size
				fseek(f, 0, SEEK_END);
				file_len = ftell(f);
				fseek(f, 0, SEEK_SET);
				printf("file size: %d bytes\n", file_len);
				buffer = (unsigned char *)malloc(file_len); //allocate
				// now time to read the contents of the file
				fread(buffer, file_len, 1, f);
				// printf("you choosed option with arg %s, %s", optarg, argv[3]);
				CheckFormat(buffer, 'n', optarg, DEFAULT2, DEFAULT3);
				fclose(f);
				break;
			case 'N':
				if(argc < 4){
					printf("option not completed -N <section name> <filename>\n");
					exit(-1);
				}
				printf("file name inserted is %s\n", argv[3]);
				f = fopen(argv[3], "rb");
				// now printing file size
				fseek(f, 0, SEEK_END);
				file_len = ftell(f);
				fseek(f, 0, SEEK_SET);
				printf("file size: %d bytes\n", file_len);
				buffer = (unsigned char *)malloc(file_len); //allocate
				// now time to read the contents of the file
				fread(buffer, file_len, 1, f);
				// printf("you choosed option with arg %s, %s", optarg, argv[3]);
				CheckFormat(buffer, 'N', optarg, DEFAULT2, DEFAULT3);
				fclose(f);
				break;
			case 'S':
				if(argc < 4){
					printf("option not completed -S <section name> <filename>\n");
					exit(-1);
				}
				printf("file name inserted is %s\n", argv[3]);
				f = fopen(argv[3], "rb");
				// now printing file size
				fseek(f, 0, SEEK_END);
				file_len = ftell(f);
				fseek(f, 0, SEEK_SET);
				printf("file size: %d bytes\n", file_len);
				buffer = (unsigned char *)malloc(file_len); //allocate
				// now time to read the contents of the file
				fread(buffer, file_len, 1, f);
				// printf("you choosed option with arg %s, %s", optarg, argv[3]);
				CheckFormat(buffer, 'S', optarg, DEFAULT2, DEFAULT3);
				fclose(f);
				break;
			case 'f':
				f = fopen(optarg, "rb");
				// // now printing file size
				fseek(f, 0, SEEK_END);
				file_len = ftell(f);
				fseek(f, 0, SEEK_SET);
				printf("file size: %d KB\n", file_len / 1024);
				buffer = (unsigned char *)malloc(file_len); //allocate
				// // now time to read the contents of the file
				fread(buffer, file_len, 1, f);
				CheckFormat(buffer, 'f', optarg, file_len, DEFAULT3);
				free(buffer);
				fclose(f);
				// fclose(fopdis);
				break;
			case 'c':
				if(argc < 4){
					printf("option not completed -c <offset> <length> <filename>\n");
					exit(-1);
				}
				printf("file name inserted is %s\n", argv[4]);
				f = fopen(argv[4], "rb");
				// now printing file size
				fseek(f, 0, SEEK_END);
				file_len = ftell(f);
				fseek(f, 0, SEEK_SET);
				printf("file size: %d bytes\n", file_len);
				buffer = (unsigned char *)malloc(file_len); //allocate
				// now time to read the contents of the file
				fread(buffer, file_len, 1, f);
				// printf("you choosed option with arg %s, %s", optarg, argv[3]);
				CheckFormat(buffer, 'c', optarg, file_len, argv[3]);
				fclose(f);
				break;
			default:
				Usage(argv[0]);
				// printf("Please Choose nicely\n");
				exit(-1);
		}
	}
	return(0);
}
