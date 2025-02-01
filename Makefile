NAME	= pipex
SRCS	= src/pipex.c \
		src/get_abs_path.c \
		src/cmds_list_handler.c \
		src/here_doc.c \
		src/print_error.c \
		src/get_next_line/get_next_line.c \
		src/get_next_line/get_next_line_utils.c

OBJS	= $(SRCS:.c=.o)
HEADERS	= include/pipex.h
CC		= cc
CFLAGS	= -Wall -Wextra -Werror
LIBFT	= ../libft/libft.a

all: $(NAME)

$(NAME): $(OBJS)
	cd "$(PWD)/../libft" && make && cd ..
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT) -I $(HEADERS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	cd "$(PWD)/../libft" && make clean && cd ..
	rm -f $(OBJS)

fclean: clean
	cd "$(PWD)/../libft" && make fclean && cd ..
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
