PROOT = $(shell pwd)
export BINDIR = $(PROOT)/bin
export OBJDIR = $(PROOT)/obj
export LIBDIR = $(PROOT)/lib

export CC = gcc
export LD = ld
export CFLAGS =-Wall -Wextra -g
export INCLUDE = -I$(PROOT)/inc
export LIBS   = -L$(LIBDIR) -lbtn

export CFLAGS +=-DDEBUG

BASE = src

all:
	+$(MAKE) -C $(BASE)/btn

clean:
	#rm -rf cscope
	rm -rf $(BINDIR)
	rm -rf $(OBJDIR)
	rm -rf $(LIBDIR)

cscope:
	mkdir -p cscope
	tools/cscope_gen.sh -d $(PROOT)/inc -d $(PROOT)/src -o $(PROOT)/cscope -b q

.PHONY: all clean test gtest mem debug cscope
