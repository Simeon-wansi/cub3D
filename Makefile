NAME =  cub3d

SRC_DIR = srcs

SRC  = $(addprefix $(SRC_DIR)/, main.c player.c)


LIBFT_DIR = includes/libft
LIBFT_LIB = $(LIBFT_DIR)/libft.a

MLX_DIR =includes/minilibx_opengl_20191021
MLX_LIB = $(MLX_DIR)/libmlx.a
MLX_INC = -I$(MLX_DIR) 
FRAMEWORK = -framework OpenGL -framework AppKit


CC = cc 
CFLAGS = -Wall -Wextra -Werror 


OBJ = $(SRC:.c=.o)

RM = rm -f

%.o: %.c
	$(CC) $(CFLAGS) $(MLX_INC) -c $< -o $@

$(NAME): $(OBJ)
	@make -C $(LIBFT_DIR)
	@echo "Compiling libft..."
	@make -C $(MLX_DIR)
	@echo "Compiling $(NAME)..."
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT_LIB) $(MLX_LIB) $(FRAMEWORK) -o $@
	@echo "$(NAME) compiled successfully!"

all: $(NAME)

clean:
	@make -C $(LIBFT_DIR) clean
	@make -C $(MLX_DIR) clean
	$(RM) $(OBJ)
	@echo "Object files cleaned."

fclean: clean
	@make -C $(LIBFT_DIR) fclean


	$(RM) $(NAME)
	@echo "$(NAME) and libft cleaned."

re: fclean all
.PHONY: all clean fclean re