NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -g

# Libft
LIBFT_DIR = libraries/libft
LIBFT = $(LIBFT_DIR)/libft.a

INCLUDES = -I include -I $(LIBFT_DIR)

# On macOS, Homebrew's GNU readline must be used explicitly because the
# system ships libedit instead, which lacks rl_replace_line and other symbols.
# On Linux (42 school), the default -lreadline links to full GNU readline.
UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
    READLINE_PREFIX := $(shell brew --prefix readline 2>/dev/null)
    INCLUDES += -I$(READLINE_PREFIX)/include
    LIBS = -L$(READLINE_PREFIX)/lib -lreadline -L$(LIBFT_DIR) -lft
else
    LIBS = -lreadline -L$(LIBFT_DIR) -lft
endif

# ============================================================================ #
#                                DIRECTORIES                                   #
# ============================================================================ #

SRC_DIR = src
OBJ_DIR = obj

PARSING_DIR = $(SRC_DIR)/parsing
EXEC_DIR = $(SRC_DIR)/execution
BUILTIN_DIR = $(SRC_DIR)/builtins
ENV_DIR = $(SRC_DIR)/env
UTILS_DIR = $(SRC_DIR)/utils
SIGNALS_DIR = $(SRC_DIR)/signals

# ============================================================================ #
#                              SOURCE FILES                                    #
# ============================================================================ #

# Main
MAIN_SRC = main.c

# Parsing (Pessoa A)
PARSING_SRC = lexer.c \
              expander.c \
			  expander_utils.c \
              parser.c \
              parser_utils.c \
              parser_free.c \
