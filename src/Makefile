# Source makefile

LIBNAME = libbase.a
CC   = clang
CCPP = clang++
CFLAGS = -Wall -Wno-unused-label
HEADERS_DIR = ../headers/

LIBFILES = utf8.o bstring.o bslist.o

.PHONY: default all clean lib

default: $(LIBFILES)
all: default

%.o: %.cpp
	$(CCPP) -isystem $(HEADERS_DIR) $(CFLAGS) -fPIC -c $< -o $@

%.o: %.c
	$(CC)   -isystem $(HEADERS_DIR) $(CFLAGS) -fPIC -c $< -o $@

lib: $(LIBFILES)
	ar -rcs $(LIBNAME) $(LIBFILES)

clean:
	-rm -f *.o
	-rm -f $(LIBNAME)
