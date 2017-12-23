PROOT = $(shell pwd)
BINDIR = $(PROOT)/bin
OBJDIR = $(PROOT)/obj
LIBDIR = $(PROOT)/lib
SRCDIR = $(PROOT)/src

MODULE = btn

CC = gcc
CXX = g++
LD = ld
CFLAGS =-Wall -Wextra -Wno-unused-parameter --std=c99 -g
CXXFLAGS =-Wall -Wextra -Wno-unused-parameter --std=c++11 -g
INCLUDE = -I$(PROOT)/inc
LIBS   = -L$(LIBDIR) -lbtn

CFLAGS +=-DDEBUG

SOURCES += $(shell find $(SRCDIR) -name '*.c')
SOURCES += $(shell find $(SRCDIR) -name '*.cpp')
BIN = lib$(MODULE).a

OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/$(MODULE)/%.o)

# unit testing stuff
GTEST_DIR = test
GTEST_BIN = test_$(MODULE)
GTEST_SRC = $(GTEST_DIR)/*.cpp
####

$(OBJDIR)/$(MODULE)/%.o:$(SRCDIR)/%.cpp
	mkdir -p $(@D)	# generate the directory
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $(@)

$(OBJDIR)/$(MODULE)/%.o:$(SRCDIR)/%.c
	mkdir -p $(@D)	# generate the directory
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $(@)

$(LIBDIR)/$(BIN): $(OBJECTS)
	mkdir -p $(LIBDIR)
	ar rcs $(LIBDIR)/$(BIN) $(OBJECTS)
	echo $(SRCDIR)
	@echo "Library built"

$(BINDIR)/$(GTEST_BIN): $(GTEST_SRC) $(OBJECTS)
	mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $(BINDIR)/$(GTEST_BIN) $(OBJECTS) $(GTEST_SRC) $(LIBS) -lgtest 
	@echo "gtest suite built"

all: $(LIBDIR)/$(BIN)

gtest: $(BINDIR)/$(GTEST_BIN)
	@$(BINDIR)/$(GTEST_BIN)

test: $(BINDIR)/$(GTEST_BIN)
	@$(BINDIR)/$(GTEST_BIN)

clean:
	#rm -rf cscope
	rm -rf $(BINDIR)
	rm -rf $(OBJDIR)
	rm -rf $(LIBDIR)

cscope:
	mkdir -p cscope
	tools/cscope_gen.sh -d $(PROOT)/inc -d $(PROOT)/src -o $(PROOT)/cscope -b q

.PHONY: all clean test gtest mem debug cscope
