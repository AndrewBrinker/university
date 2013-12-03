FLAGS=-std=c++0x -g -O2 -Wall -Wextra -Isrc -rdynamic -DNDEBUG $(OPTFLAGS)
LIBS=-ldl $(OPTLIBS)
PREFIX?=/usr/local

FILTERS=-readability/streams,-runtime/references

SOURCES=$(wildcard src/**/*.cc src/*.cc)
OBJECTS=$(patsubst %.cc,%.o,$(SOURCES))
HEADERS=$(patsubst %.cc,%.h,$(SOURCES))

TEST_SRC=$(wildcard tests/*_tests.cpp)
TESTS=$(patsubst %.cpp,%,$(TEST_SRC))

TARGET=build/docFS.a
SO_TARGET=$(patsubst %.a,%.so,$(TARGET))

# target: all       Default target. Creates all the important files.
all: $(TARGET) $(SO_TARGET) tests

dev: CFLAGS=-std=c++0x -g -Wall -Isrc -Wall -Wextra $(OPTFLAGS)
dev: all

$(TARGET): CFLAGS += -fPIC
$(TARGET): build $(OBJECTS)
	ar rcs $@ $(OBJECTS)
	ranlib $@

$(SO_TARGET): $(TARGET) $(OBJECTS)
	$(CC) -std=c++0x -shared -o $@ $(OBJECTS)

# target: build     Create the build and bin directories
build:
	@mkdir -p build
	@mkdir -p bin

# target: tests     Run cpplint.py against the source files.
.PHONY: tests
tests: CFLAGS += $(TARGET)
tests: $(TESTS)
	python tests/cpplint.py --filter=$(FILTERS) $(SOURCES) $(HEADERS)

# target: clean     Remove the compiled files.
clean:
	rm -rf build $(OBJECTS) $(TESTS)
	rm -f tests/tests.log
	find . -name "*.gc*" -exec rm {} \;
	rm -rf `find . -name "*.dSYM" -print`

# target: install   Install the library to the prefixed directory.
install: all
	install -d $(DESTDIR)/$(PREFIX)/lib/
	install $(TARGET) $(DESTDIR)/$(PREFIX)/lib/

# target: check     Check the files for potentially dangerous functions.
BADFUNCS='[^_.>a-zA-Z0-9](str(n?cpy|n?cat|xfrm|n?dup|str|pbrk|tok|_)|stpn?cpy|a?sn?printf|byte_)'
check:
	@echo Files with potentially dangerous functions.
	@egrep $(BADFUNCS) $(SOURCES) || true

# target: help      List the available targets
help:
	@egrep "^# target:" [Mm]akefile

