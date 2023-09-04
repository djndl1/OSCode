##
# Project Title
#
# @file
# @version 0.1

CC=owcc
CFLAGS=-Wall -gd -std=c99 -Iinclude -Idos_api
TARGET=-b dos
LDFLAGS=-Lbuild -llibdosapi
AR=wlib

DOS_API_SRC=dos_api/dos_api.c
BUILD_DIR=build

.PHONY: all
all: builddir libdosapi.a intr.exe

.PHONY: builddir
builddir:
	mkdir -p $(BUILD_DIR)

libdosapi.a: builddir dos_api/*.o
	$(AR) -q -n -fa $(BUILD_DIR)/$@ +dos_api/dos_api.o

dos_api/%.o : dos_api/%.c
	$(CC) -c $(CFLAGS) $(TARGET) $< -o $@

INTR_SRC=intr.c
intr.exe: builddir libdosapi.a $(INTR_SRC)
	$(CC) $(CFLAGS) $(TARGET) $(INTR_SRC) $(LDFLAGS) -o $(BUILD_DIR)/$@

docs: doxygen/Doxyfile
	doxygen $<

.PHONY: clean
clean:
	-rm -rf dos_api/*.o
	-rm -rf *.o
	-rm -rf $(BUILD_DIR)
# end
