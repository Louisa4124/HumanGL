NAME        = HumanGL
CC          = g++
CFLAGS      = -Wall -std=c++17
# CFLAGS      = -Wall -Wextra -Werror -std=c++17
IFLAGS		= -Iinclude

SRC_DIR     = src
INC_DIR     = include
OBJ_DIR     = obj

SRC         = $(wildcard $(SRC_DIR)/*.cpp)
OBJ         = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

LIBS        = -lglfw -lGL -lGLEW


all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(IFLAGS) $(OBJ) -o $(NAME) $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) $(IFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re run rerun

run: $(NAME)
	./$(NAME)

rerun: re
	./$(NAME)

format:
	clang-format -i $(SRC)
