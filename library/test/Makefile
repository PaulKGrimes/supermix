# Makefile for test programs.
# SuperMix version 1.4 C++ source file
#
# Copyright (c) 1999, 2001, 2004 California Institute of Technology.
# All rights reserved.

# Create a string to identify this architecture / OS combination.
ID = $(shell makefiles/getmachtype)

# Create a directory name for executables for this architecture / OS.
EXEDIR = exe.$(ID)

# Create a directory name for output for this architecture / OS.
COMPDIR = compare.$(ID)

all: test

configure:
	makefiles/conf $(EXEDIR) $(COMPDIR)

test: configure
	cd $(EXEDIR) ; make

clean:
	rm -f $(COMPDIR)/*.new $(COMPDIR)/*.diff

distclean: clean
	rm -rf exe.*-*-*
	rm -f compare.*-*-*/*.new compare.*-*-*/*.diff

