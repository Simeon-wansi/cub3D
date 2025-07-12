#include "../includes/cub3d.h"



int validate_texture_file(char *texture_path)
{
    FILE *file;
    char header[15];

    file = fopen(texture_path, "r");
    if (!file)
    {
        ft_printf("Warning: Texture file not found: %s\n", texture_path);
        return (0);
    }

    if (fgets(header, sizeof(header), file) == NULL)
    {
        fclose(file);
        return (0);
    }
    fclose(file);
    // checking if itls 
    return (ft_strncmp(header, "/* XPM */", 9) == 0);
}

void create_fallback_texture(t_game *game, int tex_index)
{
    int colors[4];
    int color;
    t_point point;
    char *pixel;

    printf("Debug : Creating fallback texture for index %d\n", tex_index);
    
    //create 64*64 solid texture
    game->textures[tex_index].width = 64;
    game->textures[tex_index].height = 64;
    game->textures[tex_index].img_ptr = mlx_new_image(game->mlx_ptr, 64, 64);
    if (!game->textures[tex_index].img_ptr)
    {
        ft_printf("Error: Failed to create fallback texture\n");
        return;
    }

    game->textures[tex_index].addr = mlx_get_data_addr(game->textures[tex_index].img_ptr, 
                                                        &game->textures[tex_index].bpp, 
                                                        &game->textures[tex_index].line_length, 
                                                        &game->textures[tex_index].endian);
    // Full with different colors
    colors[0] = COLOR_RED;
    colors[1] = COLOR_GREEN;
    colors[2] = COLOR_BLUE;
    colors[3] = COLOR_YELLOW;
    color = colors[tex_index % 4];

    point.y = 0;
    while (point.y < 64)
    {
        point.x = 0;
        while( point.x < 64)
        {
            pixel = game->textures[tex_index].addr + (point.y * game->textures[tex_index].line_length + point.x * (game->textures[tex_index].bpp / 8));
            *(int*)pixel = color; // Set pixel color
            point.x++;
        }
        point.y++;
    }
}


void load_texture(t_game *game)
{
    int i;
    int progress;
    char *texture_paths[4] = {
        game->map.we_texture_path,
        game->map.ea_texture_path,
        game->map.no_texture_path,
        game->map.so_texture_path
    };

    game->loading.total_textures = 4;
    game->loading.textture_loaded = 0;
    game->loading.loading_complete = false;
    printf("Debug: Loading textures from paths ...:\n");
    i = 0;
    while(i < 4)
    {
        progress = (i * 100) / 4;
        show_loading_progress(game, progress);
        
        if (validate_texture_file(texture_paths[i]))
        {
            game->textures[i].img_ptr = mlx_xpm_file_to_image(game->mlx_ptr, texture_paths[i],
                &game->textures[i].width, &game->textures[i].height);
            if (game->textures[i].img_ptr)
            {
                game->textures[i].addr = mlx_get_data_addr(game->textures[i].img_ptr, 
                                                            &game->textures[i].bpp, 
                                                            &game->textures[i].line_length, 
                                                            &game->textures[i].endian);
                if (game->textures[i].addr)
                {   
                    printf("Debug: Texture %d loaded successfully from %s\n", i, texture_paths[i]);
                    game->loading.textture_loaded++;
                }
                else 
                {
                    printf("Warining : Failed to load texture %d from %s, usinf fallbck\n", i, texture_paths[i]);
                    create_fallback_texture(game, i);
                    game->loading.textture_loaded++;
                }
            }
            else
            {
                printf("Warning: Failed to het data address fot texture %d , using fallback\n", i);
                create_fallback_texture(game, i);
                game->loading.textture_loaded++;
            }
        }
        else
        {
            printf("Warning: invalid texture file at %s, using fallback\n", texture_paths[i]);
            create_fallback_texture(game, i);
            game->loading.textture_loaded++;
        }
        i++;        
    }

    // Show completion
    show_loading_progress(game, 100);
    game->loading.loading_complete = true;
    printf("Debug: All textures loaded successfully, total loaded: %d/%d\n", game->loading.textture_loaded, game->loading.total_textures);

}


void show_loading_progress( t_game *game , int progress)
{
    int bar_width;
    int bar_height;
    t_point bar;
    t_point point;
    int fill_width;

    fast_clear_image(game);

    bar_width = WINDOW_WIDTH / 2;
    bar_height = 20;
    bar.x = WINDOW_HEIGHT / 2;
    bar.y  = WINDOW_WIDTH / 2 ;

    //draw the loading bar background
    point.y =  bar.y ;
    while( point.y < bar.y + bar_height)
    {
        point.x = bar.x;
        while(point.x < bar.x + bar_width)
        {
            put_pixel(point.x, point.y, COLOR_WHITE, game);
            point.x++;
        }
        point.y++;
    }
    fill_width = (bar_width * progress) / 100;
    point.y =  bar.y ;
    while( point.y < bar.y + bar_height)
    {
        point.x = bar.x;
        while(point.x < bar.x + fill_width)
        {
            put_pixel(point.x, point.y, COLOR_GREEN, game);
            point.x++;
        }
        point.y++;
    }
    mlx_put_image_to_window(game->mlx_ptr, game->win_ptr, game->win_img.img_ptr, 0, 0);
    printf("Debug: Loading progress: %d%%\n", progress);
} 