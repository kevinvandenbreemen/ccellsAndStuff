MAIN_FILE = main.c
TEST_FILE = test.c
PERF_FILE = tests/perftest.c
OUTPUT_DIR = ./bin
C_SRC_DIR = ./include
SWFT_SRC_C_DIR = ./Sources/cCellsAndStuff/include
REQUIRED_PKG_CHECK = libsubunit

.PHONY: cleanup setup build run checkRqdPackages buildTest test buildDebugSymbols debug checkValgrind buildPerfTest checkMemoryLeaks pipeLine swiftClean swiftPrepare swiftBuild swiftTest

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
	gcc -Wall -DNDEBUG -g -o $(OUTPUT_DIR)/run $(PERF_FILE)

checkValgrind:
	ls /usr/bin/valgrind && echo "Valgrind found"

checkMemoryLeaks: checkValgrind buildPerfTest
	valgrind --leak-check=yes --error-exitcode=1 $(OUTPUT_DIR)/run && echo "Memory leak check completed successfully 🤓"

swiftClean:
	rm -rf ./.build && rm -rf $(SWFT_SRC_C_DIR)/include

swiftPrepare: swiftClean
	mkdir $(SWFT_SRC_C_DIR)/include && cp $(C_SRC_DIR)/* $(SWFT_SRC_C_DIR)/include

swiftBuild: swiftPrepare
	swift build

swiftTest: swiftPrepare
	swift test

pipeLine: build test checkMemoryLeaks swiftTest swiftBuild
	echo "The pipeline has completed 🚀"