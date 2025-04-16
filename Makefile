CC = cc
CFLAGS = -Wall -Wextra -Werror -Iincludes
NAME = minishell
SRCS = src/execution/echo.c
OBJS = $(SRCS:.c=.o)
RM = rm -f

all: $(NAME)

$(NAME): $(OBJS)
	make -C ./libft
	$(CC) $(OBJS) ./libft/libft.a -lreadline -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	make -C ./libft clean
	$(RM) $(OBJS)

fclean: clean
	make -C ./libft fclean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re