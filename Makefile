all: world
CXX?=g++
CXXFLAGS?=--std=c++17 -Wall
INCLUDES:=-I./include -I.

OBJS:= \
	objs/logo.o \
	objs/env.o \
	objs/release.o \
	objs/main.o

LIBS:=

include cmdparser/Makefile.inc

HARDCODED_RELEASE?=""

EXTRA_CXXFLAGS:= \
	-DRELEASE_NAME=\"$(HARDCODED_RELEASE)\"

world: banner

objs/logo.o: src/logo.cpp
	 $(CXX) $(CXXFLAGS) $(INCLUDES) $(EXTRA_CXXFLAGS) -c -o $@ $<;

objs/env.o: src/env.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(EXTRA_CXXFLAGS) -c -o $@ $<;

objs/release.o: src/release.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(EXTRA_CXXFLAGS) -c -o $@ $<;

objs/main.o: main.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(EXTRA_CXXFLAGS) -c -o $@ $<;

banner: $(CMDPARSER_OBJS) $(OBJS)
	$(CXX) $(CXXFLAGS) $(EXTRA_CXXFLAGS) $(LDFLAGS) $(LIBS) $^ -o $@;

clean:
	rm -f objs/*.o banner
