
Here is the process for adding a fixed texture ("a") and a conditional texture ("b") that appears when you press the spacebar.
How It Works
We will load two new texture files when the game starts. Then, inside the main game_loop, after drawing the 3D world and the minimap, we will draw these textures directly onto the window. This ensures they always appear on top of the game world.A boolean flag in our main t_game struct will keep track of whether texture "b" should be visible or not. Pressing the spacebar will simply toggle this flag.
Step-by-Step Implementation Plan
Step 1: Update Header File (includes/cub3d.h)
First, we need to add pointers for our new textures and a state flag for texture "b" to our t_game struct. We also need to define the keycode for the spacebar.I will now modify cub3d.h to make these changes.
cub3d.h
+4
# define KEY_RIGHT 124
    t_mouse     mouse;
}   t_game;
# define KEY_RIGHT 124
# define KEY_SPACE 49
    t_mouse     mouse;
    t_texture   ui_texture_a;
    t_texture   ui_texture_b;
    bool        show_texture_b;
}   t_game;
226 hidden lines
Great, the header file is updated.
Step 2: Load the UI Textures
Next, we need to load the textures from their .xpm files when the game starts. We'll do this in a new function, init_ui_textures, which we will call from init_game.For this example, I will assume you have two texture files:
./textures/ui_a.xpm
./textures/ui_b.xpm
You can replace these paths with your actual texture files.I will now create the init_ui_textures function in srcs/textures.c.
textures.c
+22-16
        }
        else
        {
            create_fallback_texture(game, i);
            game->loading.textures_loaded++;
        }
        if (validate_texture_file(game->tex_paths[i]))
        {
            game->textures[i].img_ptr = mlx_xpm_file_to_image(game->mlx_ptr,
                                                                game->tex_paths[i],
                                                                &game->textures[i].width,
                                                                &game->textures[i].height);
            if (game->textures[i].img_ptr)
                get_texture_data_addr(game, i);
            else
                create_fallback_texture(game, i);
            game->loading.textures_loaded++;
        }
        else
        {
            game->loading.textures_loaded++;
        }
        game->textures[i].img_ptr = mlx_xpm_file_to_image(game->mlx_ptr,
                game->tex_paths[i], &game->textures[i].width,
                &game->textures[i].height);
        if (!game->textures[i].img_ptr)
            create_fallback_texture(game, i);
        else
Now, let's call init_ui_textures from our main init_game function in srcs/init.c. I'll place it after the other initializations.Okay, I'll call init_ui_textures from init_game. Now, I'll edit srcs/init.c to place it after the other initializations.
init.c
+1
    init_mouse(game);
}
    init_mouse(game);
    init_ui_textures(game);
}
Step 3: Handle the Spacebar Press
Now, we need to modify the key_press function in srcs/player.c to handle the spacebar. When the spacebar is pressed, it will toggle the show_texture_b flag.
player.c
+2
        player->move_right = true;
    else if (key_code == KEY_ESC)
        player->move_right = true;
    else if (key_code == KEY_SPACE)
        game->show_texture_b = !game->show_texture_b;
    else if (key_code == KEY_ESC)
Step 4: Draw the Textures to the Screen
Finally, we need to draw the textures in our game_loop. We'll create a new function called draw_ui for this, and call it at the end of the game_loop in srcs/raycasting.c. This ensures the UI is drawn on top of everything else.First, I'll add the draw_ui function to the srcs/drawing.c file. For this example, I'll draw texture "a" in the top-left corner and texture "b" in the top-right corner.
drawing.c
+60-25
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 17:52:00 by sngantch          #+#    #+#             */
/*   Updated: 2025/07/17 21:51:50 by sngantch         ###   ########.fr       */
/*                                                                            */
{
    int pixel_index;
/*   Created: 2025/07/16 17:52:00 by sngantch          #+#    #+#             */
/*   Updated: 2025/07/17 21:51:50 by sngantch         ###   ########.fr       */
    int pixel_index;
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 21:51:30 by sngantch          #+#    #+#             */
/*   Updated: 2025/07/17 21:51:31 by sngantch         ###   ########.fr       */
/*                                                                            */
{
    char    *pixel_ptr;
5 hidden lines
5 hidden lines
19 hidden lines
1 hidden lines
3 hidden lines
It looks like there were some other unrelated changes in srcs/drawing.c that got picked up. I will revert those changes and just add the draw_ui function.
Reapplied changes
drawing.c
Now that draw_ui is correctly defined, I'll add the call to it in the game_loop in srcs/raycasting.c. It should be one of the last things called before the track_performance function to ensure the UI is drawn on top of everything else.
raycasting.c
+2
            0, 0);
    track_performance(game);
}
            0, 0);
    draw_ui(game);
    track_performance(game);
}
Finally, I need to add the prototype for draw_ui to the cub3d.h header file.
cub3d.h
+1
void    init_mouse(t_game *game);
void    init_mouse(t_game *game);
void    draw_ui(t_game *game);
We have now completed all the necessary steps to add the UI textures.To summarize, here's what we did:
Updated cub3d.h to include the new texture pointers, the boolean flag for texture "b", and the necessary function prototypes.
Created init_ui_textures in srcs/textures.c to load the UI textures at startup.
Modified key_press in srcs/player.c to handle the spacebar press, which toggles the visibility of texture "b".
Created draw_ui in srcs/drawing.c to draw the textures to the screen.
Called draw_ui from the game_loop in srcs/raycasting.c.
Now, you just need to ensure you have the texture files textures/ui_a.xpm and textures/ui_b.xpm in your project, and then you can compile and run the game to see the results.
Review changes
 
5 of 5 To-dos
9 files edited
+966-782
Reject all Ctrl+Shift+⌫
Accept all ^⏎
Add Context




