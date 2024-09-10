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


CC = gcc
EXEC ?= main

CCFLAGS := -g -O2 -std=c99 -Wall -Wextra -pedantic

LIBFLAGS := -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm

SRC_DIR := src
INCLUDE_DIR := include
SRC_DATAS := datas_src
SRC_GAME := game_src

BUILD_DIR := ./build

SRC := $(wildcard $(SRC_DIR)/*.c)
#SRC := $(SRC_DIR)/main.c
SRC += $(wildcard $(SRC_DATAS)/*.c)
SRC += $(wildcard $(SRC_GAME)/*.c)
OBJECTS := $(addprefix $(BUILD_DIR)/, $(SRC))
OBJECTS := $(addsuffix .o, $(OBJECTS))

$(BUILD_DIR)/%.c.o: %.c
	mkdir  -p $(BUILD_DIR)/$(SRC_DIR)
	mkdir  -p $(BUILD_DIR)/$(SRC_DATAS)
	mkdir  -p $(BUILD_DIR)/$(SRC_GAME)
	$(CC) $(CCFLAGS) -c $< -o $@ $(CCFLAGS) -I$(INCLUDE_DIR) -I$(SRC_DIR)

$(EXEC): $(OBJECTS)
	$(CC) -I$(INCLUDE_DIR) -I$(SRC_DIR) $(OBJECTS) $(CCFLAGS) $(LIBFLAGS) -o $(EXEC) $(CCFLAGS)

clean:
	rm -r build
	rm $(EXEC)
