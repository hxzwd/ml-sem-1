#!/bin/bash


#FILES=$(find src/ -type f)
#SOURCES=($(echo $FILES | grep -E ".cpp$" | tr -s "\n" " "))
#HEADERS=($(echo $FILES | grep -E ".h$" | tr -s "\n" " "))


make -C src/ $@


