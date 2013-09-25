CLASSES    = VirtualDisk
EXECUTABLE = run_this
SOURCES    = $(CLASSES).cc $(CLASSES).h test/$(CLASSES).cpp
CC         = g++
CFLAGS     = -c -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -Wno-unused
LDFLAGS    =
OBJECTS    = $(SOURCES:.cc=.o)

# target: all - Default target. Does nothing.
all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cc.o:
	$(CC) $(CFLAGS) $< -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

# target: clean - Removes unecessary files.
clean:
	rm -rf *.o $(EXECUTABLE)

# target: help - Display callable targets.
help:
	egrep "^# target:" [Mm]akefile
