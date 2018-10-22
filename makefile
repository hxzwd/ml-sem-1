.SUFFIXES:
.SUFFIXES: .cpp .o .h

CC=g++
DEBUG=gdb
DEBUG_FLAGS=-x debug_cmds.txt
CFLAGS=-c -std=c++11
LDFLAGS=

SRCPATH=src
OBJPATH=obj

SOURCES=pscx_bios.cpp pscx_cpu.cpp pscx_instruction.cpp pscx_interconnect.cpp pscx_main.cpp pscx_memory.cpp pscx_ram.cpp pscx_disasm.cpp
HEADERS=pscx_bios.h pscx_cpu.h pscx_instruction.h pscx_interconnect.h pscx_common.h pscx_memory.h pscx_ram.h pscx_cachecontrol.h pscx_disasm.h
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

debug: $(EXECUTABLE)
	$(DEBUG) $(DEBUG_FLAGS)

	
