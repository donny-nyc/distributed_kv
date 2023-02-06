BUILD_DIR=build
TARGETS=$(BUILD_DIR)/controllers.o $(BUILD_DIR)/handlers.o $(BUILD_DIR)/hashing_function.o $(BUILD_DIR)/hashmap.o $(BUILD_DIR)/connections.o $(BUILD_DIR)/server.o main.c

.phony: all
all: $(TARGETS)
	gcc -g $^ -o main

$(BUILD_DIR)/server.o: infrastructure/server.c
	gcc -g -c $^ -o $@

$(BUILD_DIR)/connections.o: infrastructure/connections.c
	gcc -g -c $^ -o $@

$(BUILD_DIR)/hashmap.o: hashmap/hashmap.c
	gcc -g -c $^ -o $@

$(BUILD_DIR)/hashing_function.o: hashmap/hashing_function.c
	gcc -g -c $^ -o $@

$(BUILD_DIR)/handlers.o: handlers/http_handler.c
	gcc -g -c $^ -o $@

$(BUILD_DIR)/controllers.o: controllers/data_store_controller.c
	gcc -g -c $^ -o $@

clean:
	rm ${BUILD_DIR}/*
