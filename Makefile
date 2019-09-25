MAIN_FILE = main.c
TEST_FILE = test.c
OUTPUT_DIR = ./bin
REQUIRED_PKG_CHECK = libsubunit

.PHONY: cleanup setup build run checkRqdPackages buildTest test

cleanup:
	rm -rf $(OUTPUT_DIR)

setup:
	mkdir $(OUTPUT_DIR)

build: cleanup setup
	gcc -Wall -o $(OUTPUT_DIR)/run $(MAIN_FILE)

run: build
	$(OUTPUT_DIR)/run

checkRqdPackages:
	ldconfig -p | grep $(REQUIRED_PKG_CHECK)

buildTest: checkRqdPackages cleanup setup
	gcc $(TEST_FILE) -Wall -o $(OUTPUT_DIR)/test -lcheck -pthread -lcheck_pic -pthread -lrt -lm -lsubunit

test: buildTest
	$(OUTPUT_DIR)/test