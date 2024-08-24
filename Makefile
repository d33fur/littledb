CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Iinclude -Isrc -Isrc/libs/json/single_include/nlohmann
LDFLAGS = -lssl -lcrypto -ljsoncpp

EXEC_SERVER = src/build/littledb-server
EXEC_CLI = utils/cli/target/release/littledb-cli
EXEC_BENCHMARK_DB = benchmark/build/benchmark-db
EXEC_BENCHMARK_SERVER = benchmark/build/benchmark-server

SRC_DIR = src
OBJ_DIR = src/build
BENCHMARK_DIR = benchmark
BENCHMARK_OBJ_DIR = benchmark/build

SRC = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/**/*.cpp)
OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC))

.PHONY: all
all: clean-all build-all install-all

.PHONY: build-all
build-all: build-server build-cli build-benchmark-all

.PHONY: build-server
build-server: $(EXEC_SERVER)

$(EXEC_SERVER): $(OBJ)
	@mkdir -p $(dir $@)
	$(CXX) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: build-cli
build-cli:
	cd utils/cli && cargo build --release

.PHONY: build-benchmark-all
build-benchmark-all: build-benchmark-db build-benchmark-server

.PHONY: build-benchmark-db
build-benchmark-db: $(EXEC_BENCHMARK_DB)

$(EXEC_BENCHMARK_DB): $(BENCHMARK_DIR)/db.cpp $(filter-out $(OBJ_DIR)/main.o, $(OBJ))
	@mkdir -p $(BENCHMARK_OBJ_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)
	
.PHONY: build-benchmark-server
build-benchmark-server: $(EXEC_BENCHMARK_SERVER)

$(EXEC_BENCHMARK_SERVER): $(BENCHMARK_DIR)/server.cpp
	@mkdir -p $(BENCHMARK_OBJ_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

.PHONY: install-all
install-all: install-server install-cli install-service

.PHONY: install-server
install-server:
	install -D $(EXEC_SERVER) /usr/local/bin/littledb-server

.PHONY: install-cli
install-cli:
	install -D $(EXEC_CLI) /usr/local/bin/littledb-cli

.PHONY: install-service
install-service:
	install -D utils/littledb-service.sh /usr/local/bin/littledb-service.sh && \
	install -D utils/littledb.service /etc/systemd/system/littledb.service && \
	chmod +x /usr/local/bin/littledb-service.sh

.PHONY: clean-all
clean-all: clean-server clean-cli clean-benchmark

.PHONY: clean-server
clean-server:
	@rm -rf $(OBJ_DIR)

.PHONY: clean-cli
clean-cli:
	@cd utils/cli && \
	cargo clean 2>/dev/null

.PHONY: clean-benchmark
clean-benchmark:
	@rm -rf $(BENCHMARK_OBJ_DIR)

.PHONY: brb
brb: docker-build docker-run bash

.PHONY: docker-build
docker-build:
	docker rm -f littledb-container || true && \
	docker build -t littledb .

.PHONY: docker-run
docker-run:
	docker run -d --rm -it --name littledb-container -p 6378:6378 littledb

.PHONY: bash
bash:
	docker exec -it littledb-container /bin/bash

.PHONY: tests
tests:
	python3 tests/test_api.py

.PHONY: benchmark-all
benchmark-all: benchmark-db benchmark-server

.PHONY: benchmark-db
benchmark-db:
	./$(EXEC_BENCHMARK_DB) && \
	rm benchmark.db

.PHONY: benchmark-server
benchmark-server:
	./$(EXEC_BENCHMARK_SERVER)

.PHONY: request
request:
	nc 127.0.0.1 6378

.PHONY: server
server:
	./$(EXEC_SERVER) config.json

.PHONY: cli
cli:
	./$(EXEC_CLI)