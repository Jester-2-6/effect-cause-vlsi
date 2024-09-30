CC              = gcc
SRC_DIR         = src
INC_DIR         = include
BIN_DIR         = bin
SRC             = $(SRC_DIR)/graph.c $(SRC_DIR)/main.c $(SRC_DIR)/user.c
TARGET          = project
OBJ             = $(BIN_DIR)/graph.o $(BIN_DIR)/main.o $(BIN_DIR)/user.o
DDDEBUG         =

#------------------------------------------------------
$(TARGET): $(OBJ)
	$(CC) -o $@ $(OBJ) 

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -g -I$(INC_DIR) -c $< -o $@ $(DDDEBUG)

clean:
	rm -f $(BIN_DIR)/*.o *~ $(TARGET)
