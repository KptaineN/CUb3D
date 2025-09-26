# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -g3 -fsanitize=address
MFLAG = -L ./includes/mlx
MLX_DIR = ./includes/mlx
MLX = $(MLX_DIR)/libmlx.a -lXext -lX11 -lm
FLAGS = $(CFLAGS)

# Directories
SRC_DIR = src
LIBFT = includes/LIBFT/libft.a
HEADER_DIR = include

# Files
SRC_FILES = $(SRC_DIR)/main.c \
			$(SRC_DIR)/parser/parser.c \
			$(SRC_DIR)/parser/parser_check_elements.c \
			$(SRC_DIR)/parser/parser_map.c \
			$(SRC_DIR)/parser/check_parse_map.c \
			$(SRC_DIR)/parser/check_spawn.c \
			$(SRC_DIR)/parser/wall_checker.c \
			$(SRC_DIR)/start_game.c \
			$(SRC_DIR)/algo_math.c \
			$(SRC_DIR)/init.c \
			$(SRC_DIR)/free_memory.c \
			$(SRC_DIR)/player_init/player.c \
			$(SRC_DIR)/player_init/player_init.c \
			$(SRC_DIR)/player_init/player_init_utils.c \
			$(SRC_DIR)/draw_something.c \

OBJ_FILES = $(SRC_FILES:.c=.o)

# Executable
NAME = cub3d

# Rules
all: $(NAME)

$(NAME): $(OBJ_FILES)
	$(CC) $(FLAGS) $(OBJ_FILES) $(MLX) $(LIBFT) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -I $(HEADER_DIR) -c $< -o $@

clean:
	rm -f $(OBJ_FILES)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

