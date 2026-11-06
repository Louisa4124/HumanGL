NAME        = HumanGL
CC          = clang++
CFLAGS      = -Wall -Wextra -Werror -std=c++17
IFLAGS      = -Iinclude -Iinclude/Members

SRC_DIR     = src
OBJ_DIR     = .obj

SRC         = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*/*.cpp)
OBJ         = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

LIBS        = -lglfw -lGL -lGLEW


all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(IFLAGS) $(OBJ) -o $(NAME) $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

$(OBJ_DIR)/Members%.o: $(SRC_DIR)/Members%.cpp
	@mkdir -p $(OBJ_DIR)/Members
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re run format

run: $(NAME)
	./$(NAME)

format:
	clang-format -i $(SRC)
