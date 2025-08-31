NAME = libftpp.a

CXX = c++
CXXFLAGS = -std=c++11 -Wall -Wextra -Werror -I.

ifdef DEBUG
		CXXFLAGS += -g3 -O0 -DDEBUG
else
		CXXFLAGS += -O3
endif

SRCS_DIR = src/
SRCS = main.cpp

OBJS_DIR = obj/
OBJS = $(SRCS:%.cpp=$(OBJS_DIR)%.o)

all: $(NAME)

$(NAME): $(OBJS)
		ar rcs $(NAME) $(OBJS)

$(OBJS_DIR)%.o: $(SRCS_DIR)%.cpp
		mkdir -p $(OBJS_DIR)
		$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
		rm -fr $(OBJS_DIR)

fclean: clean
		rm -rf test/build
		rm -f $(NAME)

re: fclean all

debug:
	make -C . DEBUG=1 -B

test: all
		mkdir -p test/build
		cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -S test -B test/build
		ln -f test/build/compile_commands.json .
		make -C test/build
		./test/build/libftpp_test


.PHONY: all clean fclean re
