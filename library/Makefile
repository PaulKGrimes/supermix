# Makefile for SuperMix library and programs
# SuperMix version 1.4 C++ source file
#
# This Makefile is suitable for most users, who plan to compile
# and run SuperMix programs on a single machine architecture
# and operating system.

OBJDIR := obj

all: instructions

instructions:
	@echo
	@echo "This makefile supports the following targets:"
	@echo "  lib"
	@echo "  clean"
	@echo "  distclean"
	@echo "  profile"
	@echo

configure:
	makefiles/conf $(OBJDIR)

lib: configure
	cd $(OBJDIR) ; make

clean: configure
	cd $(OBJDIR) ; make clean

distclean: clean
	rm -rf $(OBJDIR)

profile: configure
	cd $(OBJDIR) ; make profile
