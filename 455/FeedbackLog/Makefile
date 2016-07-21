CPPFLAGS  = -std=c++11 -O2 -Wall -Wextra -Isrc -DNDEBUG $(OPTFLAGS)
LIBS      = -ldl $(OPTLIBS)
PREFIX   ?= usr/local
CC        = clang++
TARGET    = build/libfeedbacklog.a

FILTERS   = -readability/streams,-build/header_guard

SOURCES   = $(wildcard src/**/*.cc src/*.cc)
OBJECTS   = $(patsubst %.cc,%.o,$(SOURCES))
HEADERS   = $(patsubst %.cc,%.h,$(SOURCES))
TEST_SRC  = $(wildcard test/**.*.cpp test/*.cpp)
TESTS     = $(patsubst %.cpp,%,$(TEST_SRC))
SO_TARGET = $(patsubst %.a,%.so,$(TARGET))


# target: all       Compile libfeedbacklog.a
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
	@mv test/main bin/test


# target: clean     Remove compiled files
clean:
	@rm -rf build $(OBJECTS) $(TESTS)
	@find . -name "*.gc*" -exec rm {} \;
	@rm -rf `find . -name "*.dSYM" -print`


# target: install   Install libfeedbacklog.a to the system
install: all
	@install -d $(DESTDIR)/$(PREFIX)/lib/
	@install $(TARGET) $(DESTDIR)/$(PREFIX)/lib/
	@echo Installed to $(DESTDIR)/$(PREFIX)/lib


#target: lint       Check source files with bin/cpplint.py
lint:
	@python lint/cpplint.py --filter=$(FILTERS) $(SOURCES) $(HEADERS) $(TEST_SRC)


# target: help      List available targets
help:
	@egrep "^# target:" [Mm]akefile


.PHONY: tests clean install lint check
