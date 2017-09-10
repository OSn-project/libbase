# Toplevel libbase makefile

SRC_DIR = src
TESTS_DIR = tests

.PHONY: clean lib tests

default: lib

lib:
	make -C $(SRC_DIR)

tests:
	make -C $(TESTS_DIR)
	make -C $(TESTS_DIR) run

clean:
	make -C $(SRC_DIR)   clean
	make -C $(TESTS_DIR) clean
