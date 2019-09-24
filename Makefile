MAIN_FILE = main.c
OUTPUT_DIR = ./bin

.PHONY: cleanup setup build run

cleanup:
	rm -rf $(OUTPUT_DIR)

setup:
	mkdir $(OUTPUT_DIR)

build: cleanup setup
	gcc -Wall -o $(OUTPUT_DIR)/run $(MAIN_FILE)

run: build
	$(OUTPUT_DIR)/run