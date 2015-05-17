# File: makefile
# Created: 08, November 2014
#
# Copyright (C) 2015 Jarielle Catbagan

.PHONY: default clean help

MINRSHDIR	= $(PWD)/source/minrsh
MINRSHDDIR 	= $(PWD)/source/minrshd
MINTTYDIR	= $(PWD)/source/mintty
COMMONDIR	= $(PWD)/source/common
CRYPTDIR	= $(PWD)/source/crypt
CCFLAGS		= -isystem $(PWD)/include

default:
	@echo -e "error: no target(s) specified\n"
	@echo -e "specify target 'help' to see supported targets"
minrshd: $(MINRSHDDIR)/minrshd.c $(COMMONDIR)/network.c $(CRYPTDIR)/aes.c 
	gcc -o minrshd $^ $(CCFLAGS) $(ECCFLAGS)
minrsh: $(MINRSHDIR)/minrsh.c $(COMMONDIR)/network.c $(COMMONDIR)/options.c \
	$(CRYPTDIR)/aes.c
	gcc -o minrsh $^ $(CCFLAGS) $(ECCFLAGS)
clean:
	rm -f minrsh minrshd
help:
	@echo -e "targets supported:\n"
	@echo -e "\tminrsh: \tclient program"
	@echo -e "\tminrshd:\tserver program"
