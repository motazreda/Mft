CC=gcc
CFLAGS=-I

OBJS= main.c disas_func.c disasx86_capstone.c disasx86_capstone_offset.c general_analysis.c hex_dump_sections.c imports.c sections.c strings_dump.c

all:
	$(CC) $(OBJS) -lcapstone -o mft $(CFLAGS).
clean:
	rm -rf mft
