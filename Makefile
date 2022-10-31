NAME = webserv

SRCS =  srcs/cpp/main.cpp							\
		srcs/cpp/Mimes.cpp							\
		srcs/cpp/Request.cpp						\
		srcs/cpp/Server.cpp  						\
		srcs/cpp/Cgi.cpp							\
		srcs/cpp/Response.cpp						\
		srcs/cpp/Conf.cpp							\
		srcs/cpp/Error.cpp							\
		srcs/cpp/Client.cpp							\
		srcs/cpp/Open.cpp

CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -I ./headers/

HEADER = -I headers/

OBJ = $(SRCS:%.cpp=%.o)

ADDED = echo "\033[92m🛰  READY 🛰\033[0m"

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME) && $(ADDED)

MAKEFLAGS += --silent

all: $(NAME)

%.o%.c:
	$(CXX) -c $(CXXFLAGS) -I $(HEADER) $< -o ${<:.c=.o}

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re