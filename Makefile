all: world

CXX?=g++
CXXFLAGS?=--std=c++17 -Wall -g
INCLUDES+= -I./include

OBJS:= \
	objs/logo.o \
	objs/env.o \
	objs/release.o \
	objs/failsafe.o \
	objs/main.o

include common/Makefile.inc
include usage/Makefile.inc

HARDCODED_RELEASE?=""

EXTRA_CXXFLAGS+= \
	-DRELEASE_NAME=\"$(HARDCODED_RELEASE)\"

world: banner

objs/logo.o: src/logo.cpp
	 $(CXX) $(CXXFLAGS) $(INCLUDES) $(EXTRA_CXXFLAGS) -c -o $@ $<;

objs/env.o: src/env.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(EXTRA_CXXFLAGS) -c -o $@ $<;

objs/release.o: src/release.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(EXTRA_CXXFLAGS) -c -o $@ $<;

objs/failsafe.o: src/failsafe.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(EXTRA_CXXFLAGS) -c -o $@ $<;

objs/main.o: main.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(EXTRA_CXXFLAGS) -c -o $@ $<;

banner: $(COMMON_OBJS) $(USAGE_OBJS) $(OBJS)
	$(CXX) $(CXXFLAGS) $(EXTRA_CXXFLAGS) $(LDFLAGS) $^ -o $@;

.PHONY: clean
clean:
	@rm -f objs/*.o banner
