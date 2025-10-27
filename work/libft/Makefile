# Makefiles are made to automate the compilation process of multi-file
# projects, it includes the compiler, the various tags that might
# be used, the various files that have to be compiled, as well as any
# other information that might be needed > These are stored in
# variables. 
#
# The workflow of this Makefile is to take the different files contained
# in the FILES variable, compile them into object files (.o) with the "-c" tag
# using the header file (.h) contained in the same folder with "-I./"
# Then, it proceeds to use the .o files to create a .a file using the
# ar command 

CFLAGS = -Wall -Wextra -Werror

LIB = libft

COMPILER = gcc

OUTPUT = $(LIB).a

DOTCFILES = $(FILES:%=%.c)

DOTOFILES = $(FILES:%=%.o)

FILES = ft_atoi \
	ft_atol \
	ft_atoll \
	ft_bzero \
	ft_calloc \
	ft_isalnum \
	ft_isalpha \
	ft_isascii \
	ft_isdigit \
	ft_isprint \
	ft_isprint \
	ft_memchr \
	ft_memcmp \
	ft_memcpy \
	ft_memmove \
	ft_memset \
	ft_strchr \
	ft_strdup \
	ft_strlcat \
	ft_strlcpy \
	ft_strlcpy \
	ft_strlen \
	ft_strncmp \
	ft_strnstr \
	ft_strrchr \
	ft_tolower \
	ft_toupper \
	ft_substr \
	ft_strjoin \
	ft_strtrim \
	ft_split \
	ft_itoa \
	ft_strmapi \
	ft_striteri \
	ft_putchar_fd \
	ft_putstr_fd \
	ft_putendl_fd \
	ft_putnbr_fd \
	ft_parser \
	ft_print_char \
	ft_print_hex \
	ft_print_int \
	ft_print_pointer \
	ft_print_str \
	ft_print_unsigned_int \
	ft_printf \
	get_next_line \
	ft_lstnew \
	ft_lstadd_front \
	ft_lstsize \
	ft_lstlast \
	ft_lstadd_back \
	ft_lstdelone \
	ft_lstclear \
	ft_lstiter \
	ft_strstr \
	ft_isspace \

NAME = $(OUTPUT)

$(NAME):
	$(COMPILER) $(CFLAGS) -c $(DOTCFILES) -I./
	ar -rc $(OUTPUT) $(DOTOFILES)

all: $(NAME)

clean:
	@rm -f $(DOTOFILES) $(DOTOBONUS)

fclean: clean 
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re