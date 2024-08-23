CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Iinclude -Isrc/libs/json/single_include
LDFLAGS = -lssl -lcrypto -ljsoncpp

EXEC_SERVER = src/build/littledb-server
EXEC_CLI = utils/cli/target/release/littledb-cli

SRC_DIR = src
OBJ_DIR = src/build
SRC = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/**/*.cpp)
OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC))

.PHONY: all
all: build-all install-all

.PHONY: build-all
build-all: build-server build-cli

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

.PHONY: clean
clean:
	rm -rf $(OBJ_DIR) && \
	cd utils/cli && \
	cargo clean

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

.PHONY: request
request:
	nc 127.0.0.1 6378

.PHONY: server
server:
	./$(EXEC_SERVER) config.json

# .PHONY: cli
# server:
# 	./src/build/littledb-server config.json