# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: shasinan <shasinan@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/17 18:47:24 by mgodawat          #+#    #+#              #
#    Updated: 2025/06/01 14:33:47 by shasinan         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


# **************************************************************************** #
#                                COLORS                                    #
# **************************************************************************** #
GREEN = \033[0;32m
YELLOW = \033[0;33m
BLUE = \033[0;34m
PURPLE = \033[0;35m
CYAN = \033[0;36m
RED = \033[0;31m
RESET = \033[0m
BOLD = \033[1m

# **************************************************************************** #
#                               VARIABLES                                      #
# **************************************************************************** #
NAME = minishell
MAKE = /usr/bin/make
CC = cc
CFLAGS = -Wall -Wextra -Werror -Iincludes
LDFLAGS = -lreadline -lncurses

SRC_DIR = src
OBJ_DIR = objs
INCLUDES = includes
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a


SRCS = $(SRC_DIR)/main.c \
       $(SRC_DIR)/parsing/lexer/get_next_tkn.c \
       $(SRC_DIR)/parsing/lexer/lexer.c \
       $(SRC_DIR)/parsing/lexer/tkn_utils.c \
       $(SRC_DIR)/parsing/lexer/handle_quotes.c \
       $(SRC_DIR)/parsing/lexer/handle_word.c \
       $(SRC_DIR)/parsing/parser/exec_err.c \
       $(SRC_DIR)/parsing/parser/exec_node.c \
       $(SRC_DIR)/parsing/parser/exec_utils.c \
       $(SRC_DIR)/parsing/parser/free_exec.c \
       $(SRC_DIR)/parsing/parser/parser.c \
       $(SRC_DIR)/parsing/parser/parser_utils.c \
       $(SRC_DIR)/parsing/parser/append_redir.c \
       $(SRC_DIR)/parsing/parser/redirs.c \
       $(SRC_DIR)/heredoc/hd_manager.c\
       $(SRC_DIR)/heredoc/hd_read_input.c\
       $(SRC_DIR)/heredoc/hd_read_input_utils.c\
       $(SRC_DIR)/heredoc/hd_utils.c\
       $(SRC_DIR)/heredoc/hd_active_list_utils.c\
       $(SRC_DIR)/utils/err.c \
       $(SRC_DIR)/utils/exit_code.c \
       $(SRC_DIR)/utils/read.c \
       $(SRC_DIR)/utils/run_minishell.c \
       $(SRC_DIR)/utils/cleanup.c \
       $(SRC_DIR)/utils/env_utils.c \
       $(SRC_DIR)/debug/print_exec.c \
       $(SRC_DIR)/debug/print_tokens.c \
       $(SRC_DIR)/execution/builtins/echo.c \
       $(SRC_DIR)/execution/builtins/cd.c \
       $(SRC_DIR)/execution/builtins/pwd.c \
       $(SRC_DIR)/execution/builtins/env.c \
       $(SRC_DIR)/execution/builtins/export.c \
       $(SRC_DIR)/execution/builtins/unset.c \
       $(SRC_DIR)/execution/builtins/exit.c \
       $(SRC_DIR)/execution/env/init_env.c \
       $(SRC_DIR)/execution/env/update_env_var.c \
       $(SRC_DIR)/execution/exec/exec.c \
       $(SRC_DIR)/execution/exec/exec_utils.c \
       $(SRC_DIR)/execution/exec/args_to_array.c \
       $(SRC_DIR)/execution/exec/fork_and_execute.c \
       $(SRC_DIR)/execution/redir/handle_redir.c \
       $(SRC_DIR)/execution/utils/get_command.c \
       $(SRC_DIR)/execution/utils/free_all.c \
       $(SRC_DIR)/execution/utils/signals.c \

# Object files
OBJS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))

# **************************************************************************** #
#                                 RULES                                        #
# **************************************************************************** #
all: $(NAME)

# Create the executable
$(NAME): $(LIBFT) $(OBJS)
	@echo "$(BOLD)$(BLUE)🔨 Linking objects into executable...$(RESET)"
	@$(CC) $(OBJS) $(LDFLAGS) $(LIBFT) -o $(NAME)
	@echo "$(BOLD)$(GREEN)✅ Successfully built $(NAME)! 🚀$(RESET)"

# Compile libft
$(LIBFT):
	@echo -n "$(BOLD)$(PURPLE)📚 Building libft... $(RESET)"
	@$(MAKE) -s -C $(LIBFT_DIR) || \
		(echo "$(BOLD)$(RED)[FAIL]$(RESET)"; exit 1)
	@echo "$(BOLD)$(GREEN)[OK]$(RESET)"

# Compile source files
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "$(BOLD)$(YELLOW)🔄 Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "$(BOLD)$(CYAN)🧹 Cleaning object files...$(RESET)"
	@make -C $(LIBFT_DIR) clean
	@rm -rf $(OBJ_DIR)
	@echo "$(BOLD)$(GREEN)✅ Object files cleaned!$(RESET)"

fclean: clean
	@echo "$(BOLD)$(CYAN)🧹 Removing executable and libraries...$(RESET)"
	@make -C $(LIBFT_DIR) fclean
	@rm -f $(NAME)
	@echo "$(BOLD)$(GREEN)✅ Everything cleaned!$(RESET)"

re: fclean all
	@echo "$(BOLD)$(GREEN)♻️  Successfully rebuilt everything!$(RESET)"

help:
	@echo "$(BOLD)$(BLUE)Available commands:$(RESET)"
	@echo "$(BOLD)$(GREEN)make$(RESET)        - Build minishell"
	@echo "$(BOLD)$(GREEN)make clean$(RESET)  - Remove object files"
	@echo "$(BOLD)$(GREEN)make fclean$(RESET) - Remove object files and executable"
	@echo "$(BOLD)$(GREEN)make re$(RESET)     - Rebuild everything"
	@echo "$(BOLD)$(GREEN)make help$(RESET)   - Show this help message"

debug: CFLAGS += -g
debug: fclean $(NAME)
	@echo "Compilation en mode debug terminée."

# Add .PHONY to prevent conflicts with files of the same name
.PHONY: all clean fclean re help
