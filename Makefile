CC = gcc
CFLAGS = -Wall -std=c99 -Iinclude
LIBS = -lraylib

SRC_DIR = src
INC_DIR = include
BUILD_DIR = build
TARGET = c6502

# Automatically find all .c files in the SRC_DIR
SRC = $(wildcard $(SRC_DIR)/*.c)
HEADERS = $(wildcard $(INC_DIR)/*.h)

# Create corresponding .o files in the BUILD_DIR
OBJ = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC))

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

# Rule to compile .c files into .o files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create the build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

run: ${TARGET}
	./${TARGET}

format:
	clang-format -i $(SRC) $(HEADERS)