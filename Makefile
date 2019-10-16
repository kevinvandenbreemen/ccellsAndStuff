MAIN_FILE = main.c
TEST_FILE = test.c
PERF_FILE = tests/perftest.c
OUTPUT_DIR = ./bin
REQUIRED_PKG_CHECK = libsubunit

.PHONY: cleanup setup build run checkRqdPackages buildTest test buildDebugSymbols debug checkValgrind buildPerfTest checkMemoryLeaks

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

buildDebugSymbols: cleanup setup
	gcc -Wall -g -o $(OUTPUT_DIR)/run $(MAIN_FILE)

debug: buildDebugSymbols
	gdb $(OUTPUT_DIR)/run

buildPerfTest:
	gcc -Wall -g -o $(OUTPUT_DIR)/run $(PERF_FILE)

checkValgrind:
	ls /usr/bin/valgrind && echo "Valgrind found"

checkMemoryLeaks: checkValgrind buildPerfTest
	valgrind --leak-check=yes $(OUTPUT_DIR)/run