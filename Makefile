NAME	= pipex
SRCS	= src/pipex.c \
		src/get_absolute_path.c

OBJS	= $(SRCS:.c=.o)
HEADERS	= include/pipex.h
CC		= cc
CFLAGS	= -Wall -Wextra -Werror
LIBFT	= ../libft/libft.a

all: $(NAME)

$(NAME): $(OBJS)
	cd "$(PWD)/../Libft" && make && cd ..
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT) -I $(HEADERS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	cd "$(PWD)/../Libft" && make clean && cd ..
	rm -f $(OBJS)

fclean: clean
	cd "$(PWD)/../Libft" && make fclean && cd ..
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re