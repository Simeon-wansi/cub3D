/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 19:00:00 by hmensah-          #+#    #+#             */
/*   Updated: 2025/07/26 21:52:28 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_utils.h"

bool	validate_file_extension(char *filename)
{
	int	len;

	len = ft_strlen(filename);
	if (len < 4)
		return (false);
	if (ft_strcmp(filename + len - 4, ".cub") != 0)
		return (false);
	return (true);
}

bool	can_open_file(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (false);
	close(fd);
	return (true);
}

t_list	*read_file_to_list(char *filename)
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
	return (read_remaining_lines(fd, current, head));
}

t_list	*read_remaining_lines(int fd, t_list *current, t_list *head)
{
	char	*line;

	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		current->next = ft_lstnew(line);
		if (!current->next)
		{
			ft_lstclear(&head, free);
			return (close(fd), NULL);
		}
		current = current->next;
	}
	return (close(fd), head);
}

bool	verify_texture_file(char *path)
{
	int	fd;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (false);
	close(fd);
	return (true);
}
