# File: makefile
# Created: 08, November 2014
#
# Copyright (C) 2015 Jarielle Catbagan

.PHONY: default all clean help

MINRSHDIR	= $(PWD)/source/minrsh
MINRSHDDIR 	= $(PWD)/source/minrshd
MINTTYDIR	= $(PWD)/source/mintty
COMMONDIR	= $(PWD)/source/common
COREDIR		= $(PWD)/source/core
CRYPTDIR	= $(PWD)/source/crypt
CCFLAGS		= -isystem $(PWD)/include

default:
	@echo -e "error: no target(s) specified\n"
	@echo -e "specify target 'help' to see supported targets"
all: minrshd minrsh
# all executables built are manually placed in 'build'; not really elegant, so
# when time permits, find a better solution
minrshd: $(MINRSHDDIR)/minrshd.c $(COMMONDIR)/network.c \
		$(COMMONDIR)/options.c $(CRYPTDIR)/aes.c $(COREDIR)/comminit.c \
		$(COMMONDIR)/command.c
	gcc -o minrshd $^ $(CCFLAGS) $(ECCFLAGS)
	@mv $@ build
minrsh: $(MINRSHDIR)/minrsh.c $(COMMONDIR)/network.c $(COMMONDIR)/options.c \
		$(CRYPTDIR)/aes.c $(COREDIR)/comminit.c $(COMMONDIR)/command.c
	gcc -o minrsh $^ $(CCFLAGS) $(ECCFLAGS)
	@mv $@ build
clean:
# current approach must take care that 'build' is not deleted; must find a more
# elegant solution
	rm -f build/*
help:
	@echo -e "targets supported:\n"
	@echo -e "\tminrsh: \tclient program"
	@echo -e "\tminrshd:\tserver program"
