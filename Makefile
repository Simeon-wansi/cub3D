# ========== Project Configuration ==========
NAME = cub3d
CC = cc
CFLAGS = -Wall -Wextra -Werror
# Uncomment for debug mode
# CFLAGS += -g

# ========== Directory Structure ==========
SRC_DIR = srcs
OBJ_DIR = obj
INC_DIR = includes

# External libraries
LIBFT_DIR = $(INC_DIR)/libft
MLX_DIR = $(INC_DIR)/minilibx_opengl_20191021

# ========== Library Settings ==========
LIBFT_LIB = $(LIBFT_DIR)/libft.a
MLX_LIB = $(MLX_DIR)/libmlx.a

# Include paths
INCLUDES = -I$(INC_DIR) -I$(LIBFT_DIR) -I$(MLX_DIR)

# Mac-specific frameworks
FRAMEWORK = -framework OpenGL -framework AppKit

# ========== Source Files ==========
# Core files
CORE_SRC = $(SRC_DIR)/main.c

# Raycasting files
RAYCASTING_SRC = $(SRC_DIR)/raycasting/raycasting.c \
				 $(SRC_DIR)/raycasting/raycasting_utils.c \
				 $(SRC_DIR)/raycasting/render.c

# Player management
PLAYER_SRC = $(SRC_DIR)/player/player.c \
			 $(SRC_DIR)/player/collision.c

# Movement system
MOVEMENT_SRC = $(SRC_DIR)/movement/moves.c \
			   $(SRC_DIR)/movement/keys.c

# Rendering and UI
DRAW_SRC = $(SRC_DIR)/draw/drawing.c \
		   $(SRC_DIR)/draw/close_game.c \
		   $(SRC_DIR)/draw/mouse.c

# Texture management
TEXTURE_SRC = $(SRC_DIR)/textures/textures.c \
			  $(SRC_DIR)/textures/textures_utils.c

# Performance tracking
PERF_SRC = $(SRC_DIR)/performance/performance.c \
		   $(SRC_DIR)/performance/performance_utils.c \
		   $(SRC_DIR)/performance/timing.c

# Parser files
PARSER_DIR = $(SRC_DIR)/parsing
PARSER_SRC = $(PARSER_DIR)/parser.c \
			 $(PARSER_DIR)/file_handler.c \
			 $(PARSER_DIR)/string_utils.c \
			 $(PARSER_DIR)/color_parser.c \
			 $(PARSER_DIR)/texture_parser.c \
			 $(PARSER_DIR)/element_parser.c \
			 $(PARSER_DIR)/map_validator.c \
			 $(PARSER_DIR)/map_validator2.c \
			 $(PARSER_DIR)/position_validator.c \
			 $(PARSER_DIR)/player_parser.c \
			 $(PARSER_DIR)/map_parser.c \
			 $(PARSER_DIR)/map_parser2.c \
			 $(PARSER_DIR)/config_parser.c \
			 $(PARSER_DIR)/config_adapter.c \
			 $(PARSER_DIR)/map_init.c

# Combine all sources
ALL_SRCS = $(CORE_SRC) $(RAYCASTING_SRC) $(PLAYER_SRC) $(MOVEMENT_SRC) \
		   $(DRAW_SRC) $(TEXTURE_SRC) $(PERF_SRC) $(PARSER_SRC)

# Generate object file paths
OBJ = $(ALL_SRCS:%.c=$(OBJ_DIR)/%.o)

# ========== Utility Commands ==========
RM = rm -f
MKDIR = mkdir -p

# ========== Build Rules ==========
all: $(NAME)

# Main executable build
$(NAME): $(OBJ)
	@echo "Building dependencies..."
	@$(MAKE) -C $(LIBFT_DIR)
	@$(MAKE) -C $(MLX_DIR)
	@echo "Linking $(NAME)..."
	@$(CC) $(CFLAGS) $(OBJ) $(LIBFT_LIB) $(MLX_LIB) $(FRAMEWORK) -o $@
	@echo "✅ $(NAME) compiled successfully!"

# Object file generation
$(OBJ_DIR)/%.o: %.c
	@$(MKDIR) $(dir $@)
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# ========== Cleaning Rules ==========
clean:
	@$(MAKE) -C $(LIBFT_DIR) clean
	@$(MAKE) -C $(MLX_DIR) clean
	@$(RM) -r $(OBJ_DIR)
	@echo "🧹 Object files cleaned."

fclean: clean
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@$(RM) $(NAME)
	@echo "🧹 $(NAME) and libraries cleaned."

re: fclean all

.PHONY: all clean fclean re