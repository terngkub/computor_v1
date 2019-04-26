CC = g++
CFLAGS = -Wall -Wextra -Werror -std=c++11 -g3

NAME = computor
FILE =	main.cpp \
		token.cpp \
		lexer.cpp \
		ast.cpp \
		parser.cpp \
		interpreter.cpp

SRC_DIR = src
SRC = $(addprefix $(SRC_DIR)/,$(FILE))

INC = -I include

OBJ_DIR = obj
OBJ = $(addprefix $(OBJ_DIR)/,$(FILE:%.cpp=%.o))

all: $(NAME)

$(NAME): $(OBJ_DIR) $(OBJ)
	$(CC) $(CFLAGS) $(INC) -o $(NAME) $(OBJ)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
