/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 19:07:27 by hmensah-          #+#    #+#             */
/*   Updated: 2025/06/18 20:50:19 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

t_list	*build_map_line_list(char *filename)
{
	t_list	*head;
	t_list	*current;
	int		fd;
	char	*line;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (NULL);
	line = get_next_line(fd);
	if (!line)
		return (close(fd), NULL);
	head = ft_lstnew(line);
	if (!head)
		return (close(fd), NULL);
	current = head;
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break;
		current->next = ft_lstnew(line);
		current = current->next;
	}
	current->next = NULL;
	return (close(fd), head);
}

bool config_map(t_map *map, t_list *map_list, t_arena *arena)
{
	t_list	*current;
	int		i;
	int		len;

	if (!map || !map_list || !arena)
		return (false);
	map->rows = ft_lstsize(map_list);
	map->cols = 0;
	map->board = arena_alloc(arena, sizeof(char **) * (map->rows + 1));
	current = map_list;
	i = 0;
	while (current)
	{
		len = ft_strlen(current->content);
		map->board[i] = arena_alloc(arena, sizeof(char) * len);
		if (!map->board[i])
			return (false);
		if (!current->next)
			ft_strlcpy(map->board[i], current->content, len + 1);
		else
			ft_strlcpy(map->board[i], current->content, len);
		current = current->next;
		i++;
	}
	map->board[i] = NULL;
	return (true);
}

t_map	*load_map(char *filename, t_arena *arena)
{
	t_map   *map;
	t_list  *map_list;

	map = arena_alloc(arena, sizeof(t_map));
	if (!map)
		return (NULL);
	map_list = build_map_line_list(filename);
	if (!map_list)
		return (NULL);
	if (!config_map(map, map_list, arena))
	{
		ft_lstclear(&map_list, free);
		return (NULL);
	}
	ft_lstclear(&map_list, free);
	return (map);
}


void print_list_content(t_list *list)
{
	t_list *current;

	if (!list)
		return;
	current = list;
	while (current)
	{
		if (current->content)
			printf("%s", (char *)current->content);
		current = current->next;
	}
}

void print_map(t_map *map)
{
	int i;

	if (!map || !map->board)
		return;
	i = -1;
	while (++i < map->rows)
	{
		if (map->board[i])
			printf("%s\n", map->board[i]);
	}
}

int main(int argc, char **argv)
{
	t_arena *arena;
	t_list 	*head;
	t_map	*map;

	if (argc != 2 || !argv[1])
	{
		printf("Usage: %s <map_file.cub>\n", argv[0]);
		return (1);
	}
	arena = arena_create(1024 * 1024);
	if (ft_strlen(argv[1]) < 4 || ft_strncmp(argv[1] + ft_strlen(argv[1]) - 4, ".cub", 4) != 0)
	{
		printf("Error: Invalid file format. Expected a .cub file.\n");
		return (0);
	}
	head = build_map_line_list(argv[1]);
	print_list_content(head);
	printf("\n");
	printf("\n");
	map = load_map(argv[1], arena);
	if (!map)
	{
		printf("Error: Failed to load map.\n");
		return (1);
	}
	print_map(map);
	arena_destroy(arena);
	ft_lstclear(&head, free);
	return (0);
}