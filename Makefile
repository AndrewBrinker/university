CPPFLAGS  = -std=c++11 -g -O2 -Wall -Wextra -Isrc -DNDEBUG $(OPTFLAGS)
LIBS      = -ldl $(OPTLIBS)
PREFIX   ?= usr/local
CC        = clang++
TARGET    = build/libdocfs.a

SOURCES   = $(wildcard src/**/*.cc src/*.cc)
OBJECTS   = $(patsubst %.cc,%.o,$(SOURCES))
HEADERS   = $(patsubst %.cc,%.h,$(SOURCES))
TEST_SRC  = $(wildcard tests/main.cpp)
TESTS     = $(patsubst %.cpp,%,$(TEST_SRC))
SO_TARGET = $(patsubst %.a,%.so,$(TARGET))


# target: all       Compile libdocfs.a
all: $(TARGET) $(SO_TARGET)

$(TARGET): CPPFLAGS += -fPIC
$(TARGET): build $(OBJECTS)
	@ar rcs $@ $(OBJECTS)
	@ranlib $@

$(SO_TARGET): $(TARGET) $(OBJECTS)
	$(CC) -shared -std=c++11 -stdlib=libc++ -lc++abi -o $@ $(OBJECTS)

build:
	@mkdir -p build
	@mkdir -p bin


# target: tests     Compile test code
tests: CPPFLAGS += $(TARGET)
tests: $(TESTS)
	@mv tests/main bin/test


# target: clean     Remove compiled files
clean:
	@rm -rf build $(OBJECTS) $(TESTS)
	@find . -name "*.gc*" -exec rm {} \;
	@rm -rf `find . -name "*.dSYM" -print`
	@rm -rf **/*.spc *.spc **/*.dat *.dat **/test test


# target: install   Install libdocfs.a to the system
install: all
	@install -d $(DESTDIR)/$(PREFIX)/lib/
	@install $(TARGET) $(DESTDIR)/$(PREFIX)/lib/
	@echo Installed to $(DESTDIR)/$(PREFIX)/lib


#target: lint       Check source files with bin/cpplint.py
lint:
	@python lint/cpplint.py --filter=-readability/streams,-runtime/references,-runtime/printf,-runtime/explicit $(SOURCES) $(HEADERS) $(TEST_SRC)


# target: help      List available targets
help:
	@egrep "^# target:" [Mm]akefile


.PHONY: tests clean install lint check
