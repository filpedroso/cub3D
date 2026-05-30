# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+         #
#        maria-ol <maria-ol@student.42sp.org.br>  +#+#+#+#+#+   +#+            #
#    Created: 2026/05/30 13:23:40 by fpedroso          #+#    #+#              #
#    Updated: 2026/05/30 13:23:40 by fpedroso         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME = cub3D

CC := gcc
CFLAGS := -Wall -Wextra -Werror -g -Iinclude -Ilibft
# Asan: -fsanitize=address,undefined

# Directories
SRC_DIR := src
OBJ_DIR := obj
LIBFT_DIR := libft

LIBFT := $(LIBFT_DIR)/libft.a

FILES :=	main.c				\
			parser/parser.c		\
			utils/utils.c		\
			render/render.c		\
								\


SRC := $(addprefix $(SRC_DIR)/,$(FILES))
OBJ := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))


all: $(NAME)


$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME)
	@echo "cub3D compiled"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled: $<"

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)


clean:
	rm -rf $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

val: all
	valgrind	--leak-check=full		\
				--show-leak-kinds=all	\
				--trace-children=yes	\
				--track-origins=yes		\
				--track-fds=yes			\
				--keep-debuginfo=yes	\
				--tool=memcheck ./cub3D

.PHONY: all clean fclean re
