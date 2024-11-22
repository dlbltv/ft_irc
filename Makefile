CXX = c++
CXXFLAGS = -std=c++98 -Wall -Wextra -Werror
SRC = main.cpp Server.cpp Client.cpp Chanel.cpp Commands.cpp
OBJ = $(SRC:.cpp=.o)
NAME = ircserv

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJ)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
