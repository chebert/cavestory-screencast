# The C++ source files (not the headers)
SRCS=$(wildcard CaveStory/src/*.cc)
# The directory to store the compiled .o files.
OBJDIR=obj
# The .o files. One per cc file. Put them in the obj directory.
OBJECTS=$(patsubst %.cc,$(OBJDIR)/%.o,$(SRCS))
# The flags you want to use when compiling individual objects (.o files)
# -g=generate information for debugging (used with debugging tools such as gdb
#  or valgrind)
# -Wall=Give me all warnings!
# -Wextra=Give me extra warnings!
CFLAGS=-g -Wall -Wextra -std=c++03
# LDLIBS (Load Libraries)
# External libraries you are using that need to be linked.
LDLIBS=`sdl-config --cflags --libs`
# LDFLAGS (Load/linker flags)
LDFLAGS=
# The C++ compiler you are using.
CC=clang++

# The directory to put the executable.
BINDIR=gen
# What you run to play the game!
EXECUTABLE=cavestory

# Links the objects into a "cavestory" executable.
# Also links the libs in $(LDLIBS) (SDL1.2 in our case)
# Puts the Executable in the bindir.
# re-link when an object or dependency has changed.
cavestory: $(OBJECTS) depend
	@mkdir -p $(BINDIR)
	$(CC) $(LDFLAGS) -o $(BINDIR)/$(EXECUTABLE) $(OBJECTS) $(LDLIBS)

# The rule for how to compile an .o (object) file from a .cc (C++ source) file
# "-c" creates a .o file
# "-o $@" says where output the .o file
# "$<" is the .cc file
# Make the directory if it doesn't exist
$(OBJDIR)/%.o: %.cc
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGS) -o $@ $<

# This bit of magic makes it possible to recompile whats necessary when you
# change header.
# Hint: most of the magic is in the -MM flag
# BEGIN HEADER MAGIC
depend: .depend

.depend: $(SRCS)
	rm -f ./.depend
	$(CC) $(CFLAGS) -MM $^ > ./.depend;

# Includes all of the stuff that gets written to ./.depend (check it out).
include .depend
# END HEADER MAGIC

# Forces recompilation by deleting all .o files and the executable.
clean:
	rm -rf obj gen
# Just a nice way of running the game.
run:
	cd $(BINDIR) && ./$(EXECUTABLE)
