MINGW_TRIPLET=aarch64-w64-mingw32
MINGW_DIR=$(HOME)/opt/llvm-mingw
MINGW_BINARY_DIR=$(MINGW_DIR)/bin
MINGW_CXX=$(MINGW_BINARY_DIR)/$(MINGW_TRIPLET)-g++
MINGW_CC=$(MINGW_BINARY_DIR)/$(MINGW_TRIPLET)-gcc

CXX=g++
CXXFLAGS=-g -Wall
CC=gcc
CFLAGS=-g -Wall
MINGW_FLAGS=-DUNICODE -D_UNICODE
POSIX_FLAGS=-pthread

JAVAC=javac
JAVA_FLAGS=-g -Xlint

BUILDDIR=builddir

OUTPUT_EXE=$(BUILDDIR)/SimpleThread.class \
		$(BUILDDIR)/SimpleThreadWin32.exe \
		$(BUILDDIR)/SimpleThreadPosix


.PHONY: all
all: $(OUTPUT_EXE)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

.PHONY: mingw_runtime
mingw_runtime:
	cp $(MINGW_DIR)/$(MINGW_TRIPLET)/bin/*.dll $(BUILDDIR)

$(BUILDDIR)/SimpleThreadWin32.exe: SimpleThreadWin32.cc $(BUILDDIR) mingw_runtime
	$(MINGW_CXX) $(CXXFLAGS) $(MINGW_FLAGS) -o $@ $<

$(BUILDDIR)/SimpleThreadPosix: SimpleThreadPosix.cc $(BUILDDIR)
	$(CXX) $(CXXFLAGS) $(POSIX_FLAGS) -o $@ $<

$(BUILDDIR)/%.class: %.java
	$(JAVAC) $(JAVA_FLAGS) $< -d $(BUILDDIR)

.PHONY: clean
clean:
	-rm -rf $(BUILDDIR)/*.class
	-rm -rf $(BUILDDIR)/*.o
	-rm -rf $(BUILDDIR)/*.obj
	-rm -rf $(BUILDDIR)/*.exe
	-rm -rf $(OUTPUT_EXE)
