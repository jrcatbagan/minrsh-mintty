# File: makefile
# Created: 08, November 2014
#
# Copyright (C) 2015 Jarielle Catbagan

.PHONY: default clean

MINRSHDIR = $(PWD)/minrsh
MINRSHDDIR = $(PWD)/source
UTILSDIR = $(PWD)/utils

default:
	@echo "specify 'make client' or 'make server'"
minrshd: $(MINRSHDDIR)/minrshd.c $(UTILSDIR)/network.c $(UTILSDIR)/network.h
	gcc -o minrshd $(MINRSHDDIR)/minrshd.c $(UTILSDIR)/network.c $(UTILSDIR)/network.h

clean:
	rm -f client minrshd

