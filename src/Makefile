# Source makefile

LIBNAME = libbase.a
CC = clang++
CFLAGS = -Wall -Wno-unused-label
HEADERS_DIR = ../headers/

LIBFILES = bstring.o bslist.o

.PHONY: default all clean lib

default: $(LIBFILES)
all: default

%.o: %.cpp
	$(CC) -isystem $(HEADERS_DIR) $(CFLAGS) -fPIC -c $< -o $@

lib: $(LIBFILES)
	ar -rcs $(LIBNAME) $(LIBFILES)

clean:
	-rm -f *.o
	-rm -f $(LIBNAME)
