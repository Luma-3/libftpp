NAME = libftpp.a
TEST = test/build/libftpp_test

CXX = c++
CXXFLAGS = -std=c++11 -Wall -Wextra -Werror -Iinc -Itemplate

ifdef DEBUG
		CXXFLAGS += -g3 -O0 -DDEBUG
else
		CXXFLAGS += -O3
endif

SRCS_DIR = src/
SRCS =		\
		data_buffer.cpp

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

# INIT PART #

init: test/build/Makefile
		bear -- make -B -C.
		jq -s 'add' compile_commands.json test/build/compile_commands.json > compile_commands_tmp.json
		mv compile_commands_tmp.json compile_commands.json
		

# TEST PART #

CMAKE_SRCS := $(wildcard test/*.cc test/*.hh)


test/build/Makefile: $(CMAKE_SRCS) $(NAME)
		mkdir -p test/build
		cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -S test -B test/build


test: test/build/Makefile
		make -C test/build

run-test: test
		cd test/build && ctest --output-on-failure


.PHONY: all clean fclean re
