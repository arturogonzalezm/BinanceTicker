# Makefile for building and running the BinanceTicker project
.PHONY: all build up down clean logs db-shell run rebuild

# Directory variables
PROJECT = BinanceTicker
BUILD_DIR = build

# Default target
all: build up

rebuild: clean build up

# Build the Docker images without using cache
build:
	@echo "Building Docker images..."
	docker compose build --no-cache

# Start the Docker containers in detached mode
up:
	@echo "Starting Docker containers..."
	docker compose up -d

# Stop the Docker containers
down:
	@echo "Stopping Docker containers..."
	docker compose down

# Remove Docker volumes
clean:
	@echo "Removing Docker volumes..."
	docker compose down -v
	docker volume rm $(PROJECT)_pgdata || true

# Show logs for all containers
logs:
	@echo "Showing logs for all containers..."
	docker compose logs -f

# Shell into the PostgreSQL container
db-shell:
	@echo "Connecting to PostgreSQL database..."
	docker exec -it $(PROJECT)_pgdata psql -U ${DB_USER} -d ${DB_NAME}

# Build the C++ application
cpp-build: $(BUILD_DIR)/$(PROJECT)

$(BUILD_DIR)/$(PROJECT):
	@mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake -DOPENSSL_ROOT_DIR=/opt/homebrew/opt/openssl -DCMAKE_INCLUDE_PATH=/opt/homebrew/include -DCMAKE_LIBRARY_PATH=/opt/homebrew/lib ..
	cd $(BUILD_DIR) && make

# Run the C++ application
run: cpp-build
	@echo "Running the C++ application..."
	./$(BUILD_DIR)/$(PROJECT)

# Clean build artifacts
cpp-clean:
	rm -rf $(BUILD_DIR)
