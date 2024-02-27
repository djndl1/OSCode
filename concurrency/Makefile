MINGW_TRIPLET=$(shell uname -m)-w64-mingw32
MINGW_DIR=$(HOME)/opt/llvm-mingw
MINGW_BINARY_DIR=$(MINGW_DIR)/bin
MINGW_CXX=$(MINGW_BINARY_DIR)/$(MINGW_TRIPLET)-g++
MINGW_CC=$(MINGW_BINARY_DIR)/$(MINGW_TRIPLET)-gcc

CXX=g++
CXXFLAGS=-g -Wall -std=c++17
CC=gcc
CFLAGS=-g -Wall -std=c17
LDFLAGS=
MINGW_FLAGS=-DUNICODE -D_UNICODE
POSIX_FLAGS=-pthread

JAVAC=javac
JAVA_FLAGS=-g -Xlint

BUILDDIR=builddir

OUTPUT_EXE=$(BUILDDIR)/SimpleThread.class \
		$(BUILDDIR)/SimpleThreadWin32.exe \
		$(BUILDDIR)/SimpleThreadPosix \
		$(BUILDDIR)/RunSimpleThread.exe \
		$(BUILDDIR)/RunSimpleThread \
		$(BUILDDIR)/CommunicatingThread


.PHONY: all
all: $(BUILDDIR) $(OUTPUT_EXE) mingw_runtime

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

.PHONY: mingw_runtime
mingw_runtime: $(BUILDDIR)
	cp $(MINGW_DIR)/$(MINGW_TRIPLET)/bin/*.dll $(BUILDDIR)

$(BUILDDIR)/SimpleThreadWin32.exe: SimpleThreadWin32.cc
	$(MINGW_CXX) $(CXXFLAGS) $(LDFLAGS) $(MINGW_FLAGS) -o $@ $<

$(BUILDDIR)/SimpleThreadPosix: SimpleThreadPosix.cc
	$(CXX) $(CXXFLAGS) $(POSIX_FLAGS) $(LDFLAGS) -o $@ $<

$(BUILDDIR)/RunSimpleThread.exe: RunSimpleThread.cc SimpleThread.cc SimpleThread.hh
	$(MINGW_CXX) $(CXXFLAGS) $(MINGW_FLAGS) $(LDFLAGS) -o $@ $< SimpleThread.cc

$(BUILDDIR)/RunSimpleThread: RunSimpleThread.cc SimpleThread.cc SimpleThread.hh
	$(CXX) $(CXXFLAGS) $(POSIX_FLAGS) $(LDFLAGS) -o $@ $< SimpleThread.cc

$(BUILDDIR)/CommunicatingThread: CommunicatingThread.cc SimpleThread.cc SimpleThread.hh 
	$(CXX) $(CXXFLAGS) $(POSIX_FLAGS) $(LDFLAGS) -o $@ $< SimpleThread.cc

$(BUILDDIR)/%.class: %.java
	$(JAVAC) $(JAVA_FLAGS) $< -d $(BUILDDIR)

.PHONY: clean
clean:
	-rm -rf $(BUILDDIR)/*.class
	-rm -rf $(BUILDDIR)/*.o
	-rm -rf $(BUILDDIR)/*.obj
	-rm -rf $(BUILDDIR)/*.exe
	-rm -rf $(OUTPUT_EXE)
	-rm -rf $(BUILDDIR)/mingw_runtime
