# Makefile for SuperMix library and programs
# SuperMix version 1.4 C++ source file
#
# Copyright (c) 1999, 2001, 2004 California Institute of Technology.
# All rights reserved.

# This file is meant to be included by other Makefiles in the
# supermix/examples/* subdirectories, after the including file
# defines the variables EXES, DEPEND, and FLAGS
# See Makefile.generic for an example

# Set this to the directory where SuperMix is installed. It may be
# defined in the calling Makefile, in which case this definition will
# be ignored.
SUPERMIXDIR ?= ../..

# The authors recommend compiling with g++.
CC = g++

# Set the subdirectory of SUPERMIXDIR containing the shared library.
OBJDIR := obj

# Set the name and location of the SuperMix library.
LIB = libMiX.a
SUPERMIXLIB = $(SUPERMIXDIR)/$(OBJDIR)/$(LIB)

# Libraries to link to.  By default, link in SuperMix and math libraries.
# Add extras to the end of this line.
LIBS = $(SUPERMIXLIB) -lm

# Set the location of the SuperMix include files.
SINCLUDE = include
SUPERMIXINCLUDE = $(SUPERMIXDIR)/$(SINCLUDE)

# Include compiler flags here that you always want to use 
CFLAGS = $(FLAGS) -Wall -I$(SUPERMIXINCLUDE)

# clear out default implicit rule searches
.SUFFIXES:

# This rule specifies how to compile files ending in ".cc"
% : %.cc
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

# Here's the default rule to compile all files
all: $(EXES)
.PHONY: all

$(EXES): $(SUPERMIXLIB) $(DEPEND)

# Use "make clean" to rid yourself of executables
# The conditionals look to see if you are running Cygwin under MSWindows
# and remove *.exe files as well. 
.PHONY: clean
clean:
	-rm -f core $(EXES)
ifeq ($(OSTYPE), cygwin)
	-rm -f $(patsubst %, %.exe, $(EXES))
else
ifeq ($(TERM), cygwin)
	-rm -f $(patsubst %, %.exe, $(EXES))
endif
endif
