NAME = main
TEST_NAME = $(NAME)Test
CC = g++
SRC_DIR = ./src
INC_DIR = ./include
LIB_DIR = ./lib
BIN_DIR = ./bin
BUILD_DIR = ./build
TEST_DIR = ./tests

CFLAGS = -Wall -Winit-self -Wfloat-equal -Wshadow -Wswitch-default -Wconversion -O0 -I$(INC_DIR)
LFLAGS = -lglfw -lGLEW -lGL -lGLU -lm

HFILE = h
CFILE = cpp
EXE = $(BIN_DIR)/$(NAME)
EXE_TEST = $(BIN_DIR)/$(TEST_NAME)

ALL_SRCS = $(shell find $(SRC_DIR) -type f -name '*.$(CFILE)')
MAIN_SRC = $(SRC_DIR)/$(NAME).$(CFILE)
OTHER_SRCS = $(filter-out $(MAIN_SRC), $(ALL_SRCS))
OBJS = $(patsubst $(SRC_DIR)/%.$(CFILE), $(BUILD_DIR)/%.o, $(OTHER_SRCS))

ALL_TEST_SRCS = $(shell find $(TEST_DIR) -type f -name '*.$(CFILE)')
MAIN_TEST_SRC = $(TEST_DIR)/$(TEST_NAME).$(CFILE)
OTHER_TEST_SRCS = $(filter-out $(MAIN_TEST_SRC), $(ALL_TEST_SRCS))
TEST_OBJS = $(patsubst $(TEST_DIR)/%.$(CFILE), $(BUILD_DIR)/%.o, $(OTHER_TEST_SRCS))

all: $(EXE)

run: all
	$(EXE)

$(EXE): $(BUILD_DIR)/$(NAME).o $(OBJS) 
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@ $(LFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.$(CFILE)
	@mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $< -I $(INC_DIR) -o $@

test: $(EXE_TEST)

run_test: test
	$(EXE_TEST)

$(EXE_TEST): $(BUILD_DIR)/$(TEST_NAME).o $(OBJS) $(TEST_OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $^ -o $(EXE_TEST) $(LFLAGS)

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.$(CFILE)
	@mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $< -I $(INC_DIR) -I $(LIB_DIR) -o $@

leak_check: all
	valgrind --track-origins=yes --leak-check=full --show-leak-kinds=all $(EXE) > /dev/null 

heap_profiler: all
	valgrind --tool=massif --massif-out-file=massif.txt $(EXE) > /dev/null
	ms_print massif.txt > ms_out.txt

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(BIN_DIR)
	rm -f massif.txt ms_out.txt