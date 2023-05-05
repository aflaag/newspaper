CC := gcc
OBJ_DIR := bin
ROOT_DIR := src

newspaper: main.o args.o line_chunk.o line.o page.o string_tools.o
	$(CC) -o newspaper $(OBJ_DIR)/main.o $(OBJ_DIR)/args.o $(OBJ_DIR)/line_chunk.o $(OBJ_DIR)/line.o $(OBJ_DIR)/page.o $(OBJ_DIR)/string_tools.o

main.o: $(ROOT_DIR)/main.c
	$(CC) -c $(ROOT_DIR)/main.c -o $(OBJ_DIR)/main.o

args.o: $(ROOT_DIR)/args.c $(ROOT_DIR)/args.h
	$(CC) -c $(ROOT_DIR)/args.c -o $(OBJ_DIR)/args.o

line_chunk.o: $(ROOT_DIR)/line_chunk.c $(ROOT_DIR)/line_chunk.h
	$(CC) -c $(ROOT_DIR)/line_chunk.c -o $(OBJ_DIR)/line_chunk.o

line.o: $(ROOT_DIR)/line.c $(ROOT_DIR)/line.h
	$(CC) -c $(ROOT_DIR)/line.c -o $(OBJ_DIR)/line.o

page.o: $(ROOT_DIR)/page.c $(ROOT_DIR)/page.h
	$(CC) -c $(ROOT_DIR)/page.c -o $(OBJ_DIR)/page.o

string_tools.o: $(ROOT_DIR)/string_tools.c $(ROOT_DIR)/string_tools.h
	$(CC) -c $(ROOT_DIR)/string_tools.c -o $(OBJ_DIR)/string_tools.o

.PHONY: clean
clean:
	rm -f $(OBJ_DIR)/*.o newspaper