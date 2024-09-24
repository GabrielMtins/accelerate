CC = g++
EXEC ?= libacc.a

CCFLAGS := -g -O0 -std='c++11' -Wall -Wextra -pedantic

LIBFLAGS := -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm

SRC_DIR := src
INCLUDE_DIR := include

BUILD_DIR := ./build

SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(addprefix $(BUILD_DIR)/, $(SRC))
OBJECTS := $(addsuffix .o, $(OBJECTS))

$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(BUILD_DIR)/$(SRC_DIR)
	$(CC) -I$(INCLUDE_DIR) $(CCFLAGS) -c $< -o $@ $(CCFLAGS)

$(EXEC): $(OBJECTS)
	mkdir -p lib
	ar rcs lib/$(EXEC) $(OBJECTS)

clean:
	rm -rf $(BUILD_DIR)
	rm -f lib/$(EXEC)
