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

t_list	*build_list(char *filename)
{
	t_list	*head;
	t_list	*current;
	int		*fd;

	fd = open(filename, O_RDONLY, 0644);
	if (fd < 0)
		return (NULL);
	head = ft_lstnew(get_next_line(fd));
	if (!head)
		return (NULL);
	current = head;
	while (current && current->content)
	{
		current->next = ft_lstnew(get_next_line(fd));
		current = current->next;
	}
	return (head);
}

t_map	*load_map(char *filename, t_arena *arena)
{
	t_map   *map;
	t_list  *map_list;
	int     fd;

	map = arena_alloc(arena, sizeof(t_map));
	if (!map)
		return (NULL);
	map_list = build_list(filename);
	map->board = arena_alloc(arena, sizeof(char **) * (5 + 1));
	return (map);
}