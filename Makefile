# Makefile for building and running the BinanceTicker project
.PHONY: all clean run db-shell

# Directory variables
BUILD_DIR = build

# Build commands
all: $(BUILD_DIR)/BinanceTicker

$(BUILD_DIR)/BinanceTicker:
	@mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake -DOPENSSL_ROOT_DIR=/opt/homebrew/opt/openssl -DCMAKE_INCLUDE_PATH=/opt/homebrew/include -DCMAKE_LIBRARY_PATH=/opt/homebrew/lib ..
	cd $(BUILD_DIR) && make

# Run command
run: all
	$(BUILD_DIR)/BinanceTicker

# Clean command
clean:
	rm -rf $(BUILD_DIR)

db-shell:
	docker exec -it (BUILD_DIR)_pgdata psql -U postgres -d postgres
