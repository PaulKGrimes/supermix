# Makefile for SuperMix library and programs
# SuperMix version 1.4 C++ source file
#
# Copyright (c) 1999, 2001, 2004 California Institute of Technology.
# All rights reserved.

# Here's where the supermix library is installed. Change if you copy
# this Makefile into some other, arbitrary directory.
SUPERMIXDIR = ../..

# The programs we wish to compile. Simply execute "make" to build these
# programs. The command "make clean" will erase them.
EXES = hotcold matchivfts

# Dependencies for the programs we are compiling.
DEPEND = include/build_mixer.inc include/classes.h include/specs.h

# Set any extra compile flags here, such as -I<directory> to search
# additional directories for header files, etc. The -s option doesn't
# include symbol tables in executables, resulting in smaller files.
FLAGS = -s -I./include

# Here's the makefile that actually does all the work
include $(SUPERMIXDIR)/examples/makefiles/Common.mk 
