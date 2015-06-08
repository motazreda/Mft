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

 
void DisassembleCapOffset(unsigned char *buffer, char opt, unsigned char *second_option, unsigned char *third_option){

	unsigned char *length = third_option;
	unsigned char *offset = 0;

	// some unit testing ;)
	int offsets = strtoul(second_option, NULL, 16);
	offset = RealFileOffset(buffer, offsets);
	printf("the real offset is %x\n", offset);

}
