##### MAKEFILE FOR LIBFT #####

# Colors
BLACK		= \033[1;30m
RED			= \033[1;31m
GREEN		= \033[1;32m
YELLOW		= \033[1;33m
BLUE		= \033[1;34m
MAGENTA		= \033[1;35m
CYAN		= \033[1;36m
WHITE		= \033[1;37m
BOLD_RED	= \033[1;31m
NC			= \033[0m # No Color

# Compilation Variables
NAME = libft.a
CC = cc
CFLAGS = -Wextra -Wall -Werror
SRC = $(MANDATORY)
OBJ = $(SRC:.c=.o)

TEST = test

# Verbose flag
VB	= 0

# Check if VERBOSE is set to 1
ifeq ($(VB),1)
	AT =
else
	AT = @
endif

# Dependencies
MANDATORY = \
	ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c \
	ft_strlen.c ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c ft_strlcpy.c \
	ft_strlcat.c ft_toupper.c ft_tolower.c ft_strchr.c ft_strrchr.c \
	ft_strncmp.c ft_memchr.c ft_memcmp.c ft_strnstr.c ft_atoi.c \
	ft_calloc.c ft_strdup.c \
	ft_substr.c ft_strjoin.c ft_strtrim.c ft_split.c ft_itoa.c ft_strmapi.c ft_striteri.c \
	ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c \
	ft_lstnew.c \
	ft_lstadd_front.c \
	ft_lstsize.c \
	ft_lstlast.c \
	ft_lstadd_back.c \
	ft_lstdelone.c \
	ft_lstclear.c \
	ft_lstiter.c \
	ft_lstmap.c \
	ft_realloc.c \


###	Rules	#################################
$(NAME): $(OBJ)
	$(AT) ar rcs $(NAME) $(OBJ)
	@printf "\n$(WHITE)Compiling '.a' file["
	@for i in {1..16}; do \
		sleep 0.07; \
		printf "="; \
	done
	@printf "]\n\n"
	@printf "$(BLUE)Object files created.\n"
	@printf "$(GREEN)Lib created successfully.\n"

%.o: %.c
	$(AT) $(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

clean:
	$(AT) rm -f $(OBJ) $(TEST)
	@printf "$(YELLOW)Object files deleted.\n"



fclean: clean
	$(AT) rm -f $(NAME)
	@printf "$(RED)Objects and executables deleted.\n"

re: fclean all

test: $(OBJ)
	$(AT) $(CC) $(CFLAGS) $(OBJ) -o $(TEST)
	@printf "\n$(WHITE)Compiling test dependencies["
	@for i in {1..16}; do \
		sleep 0.07; \
		printf "="; \
	done
	@printf "]\n\n"
	@printf "$(GREEN)Test executable created.\n"

.PHONY: all clean fclean re test