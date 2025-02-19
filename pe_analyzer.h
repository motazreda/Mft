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
#ifndef H_PE_ANALYZER
#define H_PE_ANALYZER

#define IMAGE_DOS_SIGNATURE             0x5A4D      // MZ
#define IMAGE_OS2_SIGNATURE             0x454E      // NE
#define IMAGE_OS2_SIGNATURE_LE          0x454C      // LE
#define IMAGE_NT_SIGNATURE              0x00004550  // PE00

#define IMAGE_NUMBEROF_DIRECTORY_ENTRIES 16
#define IMAGE_SIZEOF_SHORT_NAME 8

#define IMAGE_FILE_RELOCS_STRIPPED 0x0001
#define IMAGE_FILE_EXECUTABLE_IMAGE 0x0002
#define IMAGE_FILE_NUMS_STRIPPED 0x0004
#define IMAGE_FILE_LOCAL_SYMS_STRIPPED 0x0008
#define IMAGE_FILE_AGGRESIVE_WS_TRIM 0x0010
#define IAMGE_FILE_LARGE_ADDRESS_AWARE 0x0020
#define IMAGE_FILE_BYTES_RESERVED_LO 0x0080
#define IAMGE_FILE_32BIT_MACHINE 0x0100
#define IMAGE_FILE_DEBUG_STRIPPED 0x0200
#define IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP 0x0400
#define IMAGE_FILE_NET_RUN_FROM_SWAP 0x0800
#define IMAGE_FILE_SYSTEM 0x1000
#define IMAGE_FILE_DLL 0x2000
#define IMAGE_FILE_UP_SYSTEM_ONLY 0x4000
#define IMAGE_FILE_BYTES_REVERSED_HI 0x8000

#define IMAGE_ORDINAL_FLAG32 0x80000000

typedef struct _IMAGE_DOS_HEADER {  // DOS .EXE header
    unsigned short e_magic;         // Magic number
    unsigned short e_cblp;          // Bytes on last page of file
    unsigned short e_cp;            // Pages in file
    unsigned short e_crlc;          // Relocations
    unsigned short e_cparhdr;       // Size of header in paragraphs
    unsigned short e_minalloc;      // Minimum extra paragraphs needed
    unsigned short e_maxalloc;      // Maximum extra paragraphs needed
    unsigned short e_ss;            // Initial (relative) SS value
    unsigned short e_sp;            // Initial SP value
    unsigned short e_csum;          // Checksum
    unsigned short e_ip;            // Initial IP value
    unsigned short e_cs;            // Initial (relative) CS value
    unsigned short e_lfarlc;        // File address of relocation table
    unsigned short e_ovno;          // Overlay number
    unsigned short e_res01;        // Reserved words
    unsigned short e_res02;        // Reserved words
    unsigned short e_res03;        // Reserved words
    unsigned short e_res04;        // Reserved words
    unsigned short e_oemid;         // OEM identifier (for e_oeminfo)
    unsigned short e_oeminfo;       // OEM information; e_oemid specific
    unsigned short e_res1;      // Reserved words
    unsigned short e_res2;		// Reserved words
    unsigned short e_res3;		// Reserved words
    unsigned short e_res4;		// Reserved words
    unsigned short e_res5;		// Reserved words
    unsigned short e_res6;		// Reserved words
    unsigned short e_res7;		// Reserved words
    unsigned short e_res8;		// Reserved words
    unsigned short e_res9;		// Reserved words
    unsigned short e_res10;		// Reserved words
    // unsigned short e_res2;		// Reserved words
    unsigned short e_lfanew;        // File address of new exe header
  } IMAGE_DOS_HEADER;

typedef struct _IMAGE_IMPORT_BY_NAME {
  unsigned short Hint;
  unsigned char Name[1];
} IMAGE_IMPORT_BY_NAME;

typedef struct _PIMAGE_THUNK_DATA {
  union {
    unsigned char ForwarderString;
    unsigned int Function;
    unsigned int Ordinal;
    IMAGE_IMPORT_BY_NAME AddressOfData;
  };
}PIMAGE_THUNK_DATA;

typedef struct _IMAGE_IMPORT_DESCRIPTOR {
  union {
    unsigned int Characteristics;
    PIMAGE_THUNK_DATA OriginFirstThunk;
  };
  unsigned int TimeDateStamp;
  unsigned int ForwarderChain;
  unsigned int Name;
  PIMAGE_THUNK_DATA FirstThunk;
} IMAGE_IMPORT_DESCRIPTOR;

typedef struct _IMAGE_DATA_DIRECTORY {
  unsigned int VirtualAddress;
  unsigned int Size;
} IMAGE_DATA_DIRECTORY;



typedef struct _IMAGE_EXPORT_DIRECTORY{
  unsigned int Characteristics;
  unsigned int TimeDateStamp;
  unsigned short MajorVersion;
  unsigned short MinorVersion;
  unsigned int Name;
  unsigned int Base;
  unsigned int NumberOfFunctions;
  unsigned int NumberOfNames;
  unsigned int AddressOfFunctions;
  unsigned int AddressOfNames;
  unsigned int AddressOfNameOrdinals;
}IMAGE_EXPORT_DIRECTORY;


typedef struct _IMAGE_OPTIONAL_HEADER {
  unsigned short              Magic;
  unsigned char               MajorLinkerVersion;
  unsigned char               MinorLinkerVersion;
  unsigned int                SizeOfCode;
  unsigned int                SizeOfInitializedData;
  unsigned int                SizeOfUninitializedData;
  unsigned int                AddressOfEntryPoint;
  unsigned int                BaseOfCode;
  unsigned int                BaseOfData;
  unsigned int                ImageBase;
  unsigned int                SectionAlignment;
  unsigned int                FileAlignment;
  unsigned short              MajorOperatingSystemVersion;
  unsigned short              MinorOperatingSystemVersion;
  unsigned short              MajorImageVersion;
  unsigned short              MinorImageVersion;
  unsigned short              MajorSubsystemVersion;
  unsigned short              MinorSubsystemVersion;
  unsigned int                Win32VersionValue;
  unsigned int                SizeOfImage;
  unsigned int                SizeOfHeaders;
  unsigned int                CheckSum;
  unsigned short              Subsystem;
  unsigned short              DllCharacteristics;
  unsigned int                SizeOfStackReserve;
  unsigned int                SizeOfStackCommit;
  unsigned int                SizeOfHeapReserve;
  unsigned int                SizeOfHeapCommit;
  unsigned int                LoaderFlags;
  unsigned int                NumberOfRvaAndSizes;
  IMAGE_DATA_DIRECTORY        DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
} IMAGE_OPTIONAL_HEADER;

typedef struct _IMAGE_FILE_HEADER {
    unsigned short  Machine;
    unsigned short  NumberOfSections;
    unsigned int    TimeDateStamp;
    unsigned int    PointerToSymbolTable;
    unsigned int    NumberOfSymbols;
    unsigned short  SizeOfOptionalHeader;
    unsigned short  Characteristics;
} IMAGE_FILE_HEADER;

typedef struct _IMAGE_NT_HEADERS {
	unsigned int Signature;
	IMAGE_FILE_HEADER file_header;
	IMAGE_OPTIONAL_HEADER op_header;
} IMAGE_NT_HEADERS;

typedef struct _IMAGE_SECTION_HEADER {
  unsigned char   Name[IMAGE_SIZEOF_SHORT_NAME];
  unsigned int    VirtualSize;
  unsigned int    VirtualAddress;
  unsigned int    SizeOfRawData;
  unsigned int    PointerToRawData;
  unsigned int    PointerToRelocations;
  unsigned int    PointerToLinenumbers;
  unsigned short  NumberOfRelocations;
  unsigned short  NumberOfLinenumbers;
  unsigned int    Characteristics;
} IMAGE_SECTION_HEADER;


typedef struct _IMAGE_BOUND_IMPORT_DESCRIPTOR {
  unsigned int TimeDateStamp;
  unsigned short OffsetModuleName;
  unsigned short NumberOfModuleForwarderRefs;
} IMAGE_BOUND_IMPORT_DESCRIPTOR;

#endif /* H_PE_ANALYZER */
