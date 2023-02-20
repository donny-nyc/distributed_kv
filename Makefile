BUILD_DIR=build
TARGETS=$(BUILD_DIR)/controllers.o $(BUILD_DIR)/handlers.o $(BUILD_DIR)/hashing_function.o $(BUILD_DIR)/hashmap.o $(BUILD_DIR)/connections.o $(BUILD_DIR)/server.o $(BUILD_DIR)/list.o main.c

# N.B. both libssl and libcrypto are necessary to build the mvp
LDLIBS=-lpthread -lssl -lcrypto
LDFLAGS=-I/usr/local/Cellar/openssl@3/3.0.8/include -L/usr/local/Cellar/openssl@3/3.0.8/lib

CC=gcc
CFLAGS=-v -g

.phony: all
all: $(TARGETS)
	gcc $(CFLAGS) $^ -o main $(LDFLAGS) $(LDLIBS)
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

$(BUILD_DIR)/list.o: list/list.c
	gcc -g -c $^ -o $@


secure: secure.c
	gcc $(CFLAGS) $^ -o secure $(LDFLAGS) $(LDLIBS)

clean:
	rm ${BUILD_DIR}/*
