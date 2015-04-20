# File: makefile
# Created: 08, November 2014
#
# Copyright (C) 2015 Jarielle Catbagan

.PHONY: default clean help

MINRSHDIR 	= $(PWD)/source
UTILSDIR 	= $(PWD)/utils

default:
	@echo -e "error: no target(s) specified\n"
	@echo -e "specify target 'help' to see supported targets"
minrshd: $(MINRSHDIR)/minrshd.c $(UTILSDIR)/network.c $(UTILSDIR)/network.h
	gcc -o minrshd $(MINRSHDIR)/minrshd.c $(UTILSDIR)/network.c $(UTILSDIR)/network.h
minrsh: $(MINRSHDIR)/minrsh.c $(UTILSDIR)/network.c $(UTILSDIR)/network.h
	gcc -o minrsh $(MINRSHDIR)/minrsh.c  $(UTILSDIR)/network.c $(UTILSDIR)/network.h
clean:
	rm -f minrsh minrshd
help:
	@echo -e "targets supported:\n"
	@echo -e "\tminrsh: \tclient program"
	@echo -e "\tminrshd:\tserver program"
