CPPFLAGS=-std=c++11 -g -O2 -Wall -Wextra -Isrc -DNDEBUG $(OPTFLAGS)
LIBS=-ldl $(OPTLIBS)
PREFIX?=usr/local
CC=clang++

SOURCES=$(wildcard src/**/*.cc src/*.cc)
OBJECTS=$(patsubst %.cc,%.o,$(SOURCES))
HEADERS=$(patsubst %.cc,%.h,$(SOURCES))

TEST_SRC=$(wildcard tests/main.cpp)
TESTS=$(patsubst %.cpp,%,$(TEST_SRC))

TARGET=build/libdocfs.a
SO_TARGET=$(patsubst %.a,%.so,$(TARGET))

# target: all       Default target. Creates all the important files.
all: $(TARGET) $(SO_TARGET)

dev: CPPFLAGS=-g -Wall -Isrc -Wall -Wextra $(OPTFLAGS)
dev: all

$(TARGET): CPPFLAGS += -fPIC
$(TARGET): build $(OBJECTS)
	@ar rcs $@ $(OBJECTS)
	@ranlib $@

$(SO_TARGET): $(TARGET) $(OBJECTS)
	$(CC) -shared -std=c++11 -stdlib=libc++ -lc++abi -o $@ $(OBJECTS)

# target: build     Create the build and bin directories
build:
	@mkdir -p build
	@mkdir -p bin

# target: tests     Compile the test code.
.PHONY: tests
tests: CPPFLAGS += $(TARGET)
tests: $(TESTS)
	@mv tests/main bin/test

# target: clean     Remove the compiled files.
.PHONY: clean
clean:
	@rm -rf build $(OBJECTS) $(TESTS)
	@find . -name "*.gc*" -exec rm {} \;
	@rm -rf `find . -name "*.dSYM" -print`
	@rm -rf **/*.spc *.spc **/*.dat *.dat **/test test

# target: install   Install the library to the prefixed directory.
.PHONY: install
install: all
	@install -d $(DESTDIR)/$(PREFIX)/lib/
	@install $(TARGET) $(DESTDIR)/$(PREFIX)/lib/
	@echo Installed to $(DESTDIR)/$(PREFIX)/lib

#target: lint       Check the files against Google's C++ StyleGuide.
.PHONY: lint
lint:
	@python lint/cpplint.py --filter=-readability/streams,-runtime/references,-runtime/printf,-runtime/explicit $(SOURCES) $(HEADERS) $(TEST_SRC)

# target: check     Check the files for potentially dangerous functions.
BADFUNCS='[^_.>a-zA-Z0-9](str(n?cpy|n?cat|xfrm|n?dup|str|pbrk|tok|_)|stpn?cpy|a?sn?printf|byte_)'
.PHONY: check
check:
	@echo Files with potentially dangerous functions:
	@egrep $(BADFUNCS) $(SOURCES) || true
	@echo Check complete.

# target: help      List the available targets
help:
	@egrep "^# target:" [Mm]akefile

