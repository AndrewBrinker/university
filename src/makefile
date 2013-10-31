SOURCES    = VirtualDisk.cc \
             FileSystem.cc \
             Table.cc \
             test/main.cpp
TESTS      = test/VirtualDiskTest.cpp \
             test/FileSystemTest.cpp \
             test/TableTest.cpp
EXECUTABLE = run_this
CC         = g++
INCLUDES   = test/
CFLAGS     = -c -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-promo -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -Wno-unused
LDFLAGS    =
OBJECTS    = $(SOURCES:.cc=.o)

SHELL = /bin/sh

.SUFFIXES:
.SUFFIXES: .cc .h .cpp .o

# target: all - Default target. Compiles all sources and tests.
all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cc.o:
	$(CC) -I $(INCLUDES)  $(CFLAGS) $< -o $@

.cpp.o:
	$(CC) -I $(INCLUDES) $(CFLAGS) $< -o $@

# target: prep - Removes unecessary files.
prep:
	rm -rf *.o *.spc *.dat $(EXECUTABLE)

# target: lint - Runs cpplint.py on all files.
lint:
	python test/cpplint.py --filter=-readability/streams,-runtime/references $(SOURCES) $(TESTS)

# target: save - Commit changes to the source files.
save:
	git add $(SOURCES) $(TESTS) && git commit -m "Autosaving"

# target: push - Push the changes the repository
push:
	git push

# target: help - Display callable targets.
help:
	egrep "^# target:" [Mm]akefile

# target: list - An alias for 'make help'.
list:
	egrep "^# target:" [Mm]akefile
