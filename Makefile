CFLAGS = -Wno-register -O2 -lm -std=c++17 -Icode -g

BUILD_DIR ?= ./build

CPPS = \
	code/retrieval.cpp

main: test.cpp $(CPPS)
	mkdir -p $(BUILD_DIR)
	g++ $(CPPS) test.cpp -o $(BUILD_DIR)/system_main $(CFLAGS)

clean:
	rm -rf $(BUILD_DIR)
