SRCDIR=CaveStory/src
# The C++ source files (but not the headers)
SRCS=$(wildcard $(SRCDIR)/*.cc)

# The directory to store the compiled .o files.
OBJDIR=obj

# The .o files. One per cc file. Put them in the obj directory.
OBJECTS=$(patsubst %.cc,$(OBJDIR)/%.o,$(SRCS))

# The flags you want to use when compiling individual objects (.o files)
# -g=generate information for debugging (used with debugging tools such as gdb
#  or valgrind)
# -Wall=Give me all warnings!
# -Wextra=Give me extra warnings!
# -std=c++03=Enforce C++03 standard compliance. (You could also use C++11 if you
#  want to be more up-to-date).
# -MMD=Create a .d file to store the rule for the header dependencies of each object.
CFLAGS=-g -Wall -Wextra -std=c++03 -MMD

# LDLIBS (Load Libraries)
# External libraries you are using that need to be linked.
# ``=run a shell command (command substitution)
# sdl-config=a command that generates the load libs/cflags necessary depending
# on the platform (OS/Linux/Win)
LDLIBS=`sdl-config --cflags --libs` -lboost_system -lboost_filesystem

# LDFLAGS (Load/linker flags)
LDFLAGS=

# The C++ compiler you are using.
CC=clang++

# The directory to put the executable.
BINDIR=gen

# What you run to play the game!
EXECUTABLE=cavestory

# If files named all/clean/run exist in the directory, then
# Make will not run these commands (because they will be "Up-to-Date")
# .PHONY is a special way of telling Make to run these no matter what.
.PHONY: all clean run

all: $(EXECUTABLE)

# Pastes in all of the Make rules inside of the generated .d files.
# Rules are of the format (file.o: file.cc header1.h header2.h)
# Each .d file gets generated whenever its .o file is compiled.
# Special thanks to /u/yurik for pointing this out to me!
-include $(OBJECTS:.o=.d)

# Links the objects into a "cavestory" executable.
# Also links the libs in $(LDLIBS) (SDL1.2 in our case)
# Puts the Executable in the bindir.
# re-link when an object or dependency has changed.
$(EXECUTABLE): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CC) $(LDFLAGS) -o $(BINDIR)/$(EXECUTABLE) $(OBJECTS) $(LDLIBS)

# The rule for how to compile an .o (object) file from a .cc (C++ source) file
# "-c" creates a .o file
# "-o" says to create an object file (.o)
# "$@" in this case is the name of the object file (.o). MUST FOLLOW -o
# "$<" in this case is the .cc file
# Make the directory if it doesn't exist
$(OBJDIR)/%.o: %.cc
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGS) -o $@ $<

# Deletes all .o/.d files and the executable. This helps when you want to
# force recompilation.
clean:
	rm -rf obj gen

# Just a nice way of running the game. (Since you have to be in the bindir for
# the content to load properly)
run:
	cd $(BINDIR) && ./$(EXECUTABLE)
