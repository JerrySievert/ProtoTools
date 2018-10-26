SLUG = ProtoTools
VERSION = 0.6.0

# FLAGS will be passed to both the C and C++ compiler
FLAGS +=
CFLAGS +=
CXXFLAGS += -Ideps/duktape

# Careful about linking to libraries, since you can't assume much about the user's environment and library search path.
# Static libraries are fine.
LDFLAGS +=

# Add .cpp and .c files to the build
SOURCES += $(wildcard src/*.cpp) $(wildcard src/controller/*.cpp) $(wildcard src/view/*.cpp) $(wildcard src/lib/*.cpp) $(wildcard deps/duktape/*.c)

# Add files to the ZIP package when running `make dist`
# The compiled plugin is automatically added.
DISTRIBUTABLES += $(wildcard LICENSE*) res modules bootstrap.js

# Must include the VCV plugin Makefile framework
RACK_DIR ?= ../..
include $(RACK_DIR)/plugin.mk
