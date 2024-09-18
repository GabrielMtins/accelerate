#    Copyright (C) 2020  Gabriel Martins

#    This file is part of Neschgui.

#    Neschgui is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.

#    Neschgui is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.

#    You should have received a copy of the GNU General Public License
#    along with Neschgui.  If not, see <https://www.gnu.org/licenses/>.


CC = g++
EXEC ?= main

CCFLAGS := -g -O2 -std='c++11' -Wall -Wextra -pedantic

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
	$(CC) -I$(INCLUDE_DIR) -I./src $(OBJECTS) $(CCFLAGS) $(LIBFLAGS) -o $(EXEC) $(CCFLAGS)

clean:
	rm -r build
	rm $(EXEC)
