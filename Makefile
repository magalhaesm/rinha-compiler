NAME := interpreter

OBJ_DIR  := obj
INC_DIRS := include
SRC_DIRS := src

vpath %.hpp $(INC_DIRS)
vpath %.cpp $(SRC_DIRS)

CXX      := c++
CXXFLAGS := -Wall -Wextra -Werror -std=c++17 -Ofast

INCS := types.hpp binaryOp.hpp interpreter.hpp
SRCS := types.cpp binaryOp.cpp interpreter.cpp main.cpp
OBJS := $(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o))

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $^ -o $@

$(OBJ_DIR)/%.o: %.cpp $(INCS) | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -I $(INC_DIRS) -c $< -o $@

$(OBJ_DIR):
	@mkdir $@

clean:
	@$(RM) -r $(OBJS)

fclean: clean
	@$(RM) -r $(NAME)

re: clean all

.PHONY: all clean fclean re
