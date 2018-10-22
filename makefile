.PHONY: all debug #debug_run
.SUFFIXES:
.SUFFIXES: .cpp .o .h

CC=g++
DEBUG=gdb
DEBUG_SCRIPT=debug_cmds.txt
DEBUG_OPTIONS=--command=$(DEBUG_SCRIPT)
DEBUG_FLAGS=--args
EXECUTABLE_ARGS=roms/SCPH1001.BIN
CFLAGS=-c -std=c++11 -g
LDFLAGS=

SRCPATH=src
OBJPATH=obj

SOURCES=pscx_bios.cpp pscx_cpu.cpp pscx_instruction.cpp pscx_interconnect.cpp pscx_main.cpp pscx_memory.cpp pscx_ram.cpp pscx_disasm.cpp
HEADERS=pscx_bios.h pscx_cpu.h pscx_instruction.h pscx_interconnect.h pscx_common.h pscx_memory.h pscx_ram.h pscx_cachecontrol.h pscx_disasm.h
OBJECTS_DEBUG=$(SOURCES:.cpp=.o)
OBJECTS=$(addprefix $(OBJPATH)/,$(SOURCES:.cpp=.o))
EXECUTABLE=pscx_main

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(EXECUTABLE)

obj/%.o: src/%.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJPATH)/*.o
	rm -rf $(EXECUTABLE)

debug:
	$(DEBUG) $(DEBUG_OPTIONS) $(DEBUG_FLAGS) $(EXECUTABLE) $(EXECUTABLE_ARGS)

#debug_run:
#	$(foreach source_file,$(SOURCES),$(CC) $(CFLAGS) $(source_file) -o $(source_file:.cpp=.o))
#	$(CC) $(LDFLAGS) $(OBJECTS_DEBUG) -o $(EXECUTABLE)
#
#debug:
#	make -C debug debug_run
#	
