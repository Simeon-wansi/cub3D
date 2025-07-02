/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_util.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 19:07:27 by hmensah-          #+#    #+#             */
/*   Updated: 2025/07/02 19:20:11 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_utils.h"

void	print_error(char *message)
{
	ft_putstr_fd("Error\n", 2);
	ft_putstr_fd(message, 2);
	ft_putchar_fd('\n', 2);
}

char	*skip_whitespace(char *str)
{
	while (*str && (*str == ' ' || *str == '\t'))
		str++;
	return (str);
}

char	*trim_newline(char *str)
{
	int	len;

	if (!str)
		return (str);
	len = ft_strlen(str);
	if (len > 0 && str[len - 1] == '\n')
		str[len - 1] = '\0';
	return (str);
}

bool	is_valid_map_char(char c)
{
	return (c == '0' || c == '1' || c == 'N' || c == 'S' || c == 'E'
		|| c == 'W' || c == ' ');
}

bool	parse_color_p(char *line, int *r, int *g, int *b)
{
	char	**rgb_parts;
	int		i;

	rgb_parts = ft_split(line, ',');
	if (!rgb_parts)
		return (false);
	i = 0;
	while (rgb_parts[i])
		i++;
	if (i != 3)
	{
		i = 0;
		while (rgb_parts[i])
			free(rgb_parts[i++]);
		free(rgb_parts);
		return (false);
	}
	*r = ft_atoi(rgb_parts[0]);
	*g = ft_atoi(rgb_parts[1]);
	*b = ft_atoi(rgb_parts[2]);
	i = 0;
	while (rgb_parts[i])
		free(rgb_parts[i++]);
	return (free(rgb_parts), *r >= 0 && *r <= 255 && *g >= 0 && *g <= 255
		&& *b >= 0 && *b <= 255);
}
