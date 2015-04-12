# File: makefile
# Created: 08, November 2014
#
# Copyright (C) 2015 Jarielle Catbagan

.PHONY: default clean

default:
	@echo "specify 'make client' or 'make server'"
client: client.c network.c network.h
	gcc -o client client.c network.c network.h
server:	server.c network.c network.h
	gcc -o server server.c network.c network.h
clean:
	rm -f client server

