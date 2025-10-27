# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -I $(HEADER_DIR) -I $(MLX_DIR)

# Directories
SRC_DIR = src
HEADER_DIR = header
LIBFT_DIR = libft
MLX_DIR = includes/minilibx-linux

# Files
SRC_FILES = $(SRC_DIR)/main.c \
			$(SRC_DIR)/init.c \
			$(SRC_DIR)/parser/parser.c \
			$(SRC_DIR)/parser/parser_check_elements.c \
			$(SRC_DIR)/parser/parser_map.c \
			$(SRC_DIR)/parser/check_parse_map.c \
			$(SRC_DIR)/parser/check_spawn.c \
			$(SRC_DIR)/parser/wall_checker.c \
			$(SRC_DIR)/parser/parser_color_checker.c \
			$(SRC_DIR)/parser/parser_path_checker.c \
			$(SRC_DIR)/parser/parser_utils.c \
			$(SRC_DIR)/player/player_init.c \
			$(SRC_DIR)/player/player_init_utils.c \
			$(SRC_DIR)/player/player_move.c \
			$(SRC_DIR)/game/game_init.c \
			$(SRC_DIR)/game/game_cleanup.c \
			$(SRC_DIR)/game/game_keys.c \
			$(SRC_DIR)/game/launcher.c \
			$(SRC_DIR)/game/rayon.c \
			$(SRC_DIR)/game/dda.c \


OBJ_FILES = $(SRC_FILES:.c=.o)

NAME = cub3d

# MLX Linking flags (Linux)
MLX_FLAGS = -L$(MLX_DIR) -lmlx_Linux -L/usr/lib -lXext -lX11 -lm

all: $(NAME)

$(NAME): $(OBJ_FILES)
	@$(MAKE) -C $(LIBFT_DIR)
	@$(MAKE) -C $(MLX_DIR)
	$(CC) $(CFLAGS) $(OBJ_FILES) -L$(LIBFT_DIR) -lft $(MLX_FLAGS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@$(MAKE) -C $(LIBFT_DIR) clean
	rm -f $(OBJ_FILES)

fclean: clean
	@$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all
