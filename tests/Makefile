# Tests makefile
# NOTE: ~unittest-cpp needs to be made with the -DUTPP_USE_PLUS_SIGN=OFF option~ No it doesn't

TARGET = tests
LIBS =
CC = clang++
CFLAGS = -O0 -g -Wno-c++11-compat-deprecated-writable-strings

UTPP_LIB = /usr/local/lib/libUnitTest++.a

HEADERS_DIR = ../headers/

TEST_SUBJECTS = ../src/bstring.o ../src/bslist.o ../src/utf8.o
TEST_CODE     = bstring.test.cpp bslist.test.cpp utf8.test.cpp byteorder.test.cpp

.PHONY: default all clean

default: $(TARGET)

all: default

%.o: %.cpp
	$(CC) -isystem $(HEADERS_DIR) $(CFLAGS) -o $@ -c $<

$(TARGET): $(TEST_CODE)
	$(CC) $(TEST_SUBJECTS) $(TEST_CODE) main.cpp $(UTPP_LIB) $(CFLAGS) -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	-rm -f *.o *.a
	-rm -f $(TARGET)
