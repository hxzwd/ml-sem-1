
.PHONY=clean all mvbin cpbin cleanbin
CC=g++
CFLAGS=-c -Wall -std=c++11
LDFLAGS=
ROOT_DIR=$(shell pwd)
BIN_DIR=bin
SRC_DIR=src
SOURCE_DIRS=disasm
EXECUTABLE=pscx_main
SOURCES_ROOT=pscx_bios.cpp pscx_cpu.cpp pscx_instruction.cpp pscx_interconnect.cpp pscx_main.cpp
HEADERS_ROOT=pscx_bios.h pscx_cpu.h pscx_instruction.h pscx_interconnect.h
SOURCES_DISASM=disasm_main.cpp disasm_op.cpp
HEADERS_DISASM=disasm_main.h disasm_op.h
SOURCES=pscx_bios.cpp pscx_cpu.cpp pscx_instruction.cpp pscx_interconnect.cpp pscx_main.cpp disasm/disasm_main.cpp disasm/disasm_op.cpp
HEADERS=pscx_bios.h pscx_cpu.h pscx_instruction.h pscx_interconnect.h disasm/disasm_main.h disasm/disasm_op.h
#SOURCES=$(shell find src/ -type f | grep -E ".cpp$" | tr -s "\n" " ")
#HEADERS=$(shell find src/ -type f | grep -E ".h$" | tr -s "\n" " ")
#SOURCES=$(SOURCES_ROOT) $(SOURCES_DISASM)
#HEADERS=$(HEADERS_ROOT) $(HEADERS_DISASM)
OBJECTS=$(SOURCES:.cpp=.o)

all: $(EXECUTABLE) $(SOURCES) $(HEADERS)
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(EXECUTABLE)

.cpp.o: $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) $< -o $@

mvbin: $(OBJECTS) $(EXECUTABLE)
	mv $(OBJECTS) $(EXECUTABLE) $(ROOT_DIR)/$(BIN_DIR)/.

cpbin: $(OBJECTS) $(EXECUTABLE)
	cp $(OBJECTS) $(EXECUTABLE) $(ROOT_DIR)/$(BIN_DIR)/.

cleanbin:
	@cd $(ROOT_DIR)/$(BIN_DIR)
	rm -rf *.o $(EXECUTABLE)
	@cd $(ROOT_DIR)/$(SRC_DIR)

clean:
	rm -rf *.o $(EXECUTABLE)

