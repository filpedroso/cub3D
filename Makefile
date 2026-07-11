# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mona <mona@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/05/30 13:23:40 by fpedroso          #+#    #+#              #
#    Updated: 2026/06/07 20:28:21 by mona             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = cub3D

CC := gcc
CFLAGS := -Wall -Wextra -Werror -g -Iinclude -Ilibft -IMLX42/include
# Asan: -fsanitize=address,undefined

# Colors
GREEN        = \033[0;32m
RED          = \033[0;31m
BLUE         = \033[0;34m
YELLOW       = \033[0;33m
LILAC_TRUE   = \033[38;2;200;162;200m
RESET        = \033[0m

# Directories
SRC_DIR := src
OBJ_DIR := obj
LIBFT_DIR := libft
MLX_DIR := MLX42/build

LIBFT := $(LIBFT_DIR)/libft.a
MLX   := $(MLX_DIR)/libmlx42.a
GLFW  := MLX42/build/_deps/glfw-build/src/libglfw3.a

FILES :=	main.c				 \
			parser/parser.c		 \
			parser/parser_meta.c \
			parser/parser_map.c	 \
			parser/parser_color.c \
			parser/parser_walls.c \
			parser/file_utils.c	 \
			parser/free_utils.c	 \
			utils/utils.c		 \
			render/render.c		 \

SRC := $(addprefix $(SRC_DIR)/,$(FILES))
OBJ := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))

TEST_NAME := test_parser
TEST_OBJS :=	$(OBJ_DIR)/main_test.o			 \
				$(OBJ_DIR)/parser/parser.o		 \
				$(OBJ_DIR)/parser/parser_meta.o \
				$(OBJ_DIR)/parser/parser_map.o	 \
				$(OBJ_DIR)/parser/parser_color.o \
				$(OBJ_DIR)/parser/parser_walls.o \
				$(OBJ_DIR)/parser/file_utils.o	 \
				$(OBJ_DIR)/parser/free_utils.o	 \
				$(OBJ_DIR)/utils/utils.o		 \

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(MLX) $(GLFW) \
		-framework Cocoa -framework OpenGL -framework IOKit -o $(NAME)
	@$(MAKE) banner

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR) --no-print-directory

test: $(TEST_OBJS) $(LIBFT)
	@$(CC) $(CFLAGS) $(TEST_OBJS) $(LIBFT) $(MLX) $(GLFW) \
		-framework Cocoa -framework OpenGL -framework IOKit -o $(TEST_NAME)
	@echo "$(GREEN)Parser test binary built: ./$(TEST_NAME) <map.cub>$(RESET)"

clean:
	@rm -rf $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean --no-print-directory
	@echo "$(RED)cub3D objects removed$(RESET)"

fclean: clean
	@rm -f $(NAME) $(TEST_NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean --no-print-directory
	@echo "$(RED)cub3D deleted$(RESET)"

re: fclean all

val: all
	valgrind	--leak-check=full		\
				--show-leak-kinds=all	\
				--trace-children=yes	\
				--track-origins=yes		\
				--track-fds=yes			\
				--keep-debuginfo=yes	\
				--tool=memcheck ./cub3D

banner:
	@echo ""
	@printf '%b\n' "$(LILAC_TRUE)  ██████╗██╗   ██╗██████╗ ██████╗ ██████╗ $(RESET)"
	@printf '%b\n' "$(LILAC_TRUE) ██╔════╝██║   ██║██╔══██╗╚════██╗██╔══██╗$(RESET)"
	@printf '%b\n' "$(LILAC_TRUE) ██║     ██║   ██║██████╔╝ █████╔╝██║  ██║$(RESET)"
	@printf '%b\n' "$(LILAC_TRUE) ██║     ██║   ██║██╔══██╗ ╚═══██╗██║  ██║$(RESET)"
	@printf '%b\n' "$(LILAC_TRUE) ╚██████╗╚██████╔╝██████╔╝██████╔╝██████╔╝$(RESET)"
	@printf '%b\n' "$(LILAC_TRUE)  ╚═════╝ ╚═════╝ ╚═════╝ ╚═════╝ ╚═════╝ $(RESET)"
	@echo ""
	@echo "$(LILAC_TRUE)        🎮 First-Person Raycaster in C$(RESET)"
	@echo "$(LILAC_TRUE)        by fpedroso & mona — 42 São Paulo$(RESET)"
	@echo ""
	@echo "$(GREEN)  Compiled successfully! Ready to cast some rays! ✨$(RESET)"
	@echo ""

.PHONY: all clean fclean re val banner test
