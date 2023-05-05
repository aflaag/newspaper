CC := gcc

TARGET := newspaper

OBJ_DIR := objdir
ROOT_DIR := src

SOURCES := $(wildcard $(ROOT_DIR)/*.c)
OBJECTS := $(patsubst $(ROOT_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))

all: dirs $(TARGET)

dirs:
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(ROOT_DIR)/%.c
	$(CC) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CC) $^ -o $@

clean:
	@rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all dir clean