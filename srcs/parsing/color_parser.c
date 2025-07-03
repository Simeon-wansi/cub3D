/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 19:00:00 by hmensah-          #+#    #+#             */
/*   Updated: 2025/07/03 19:00:00 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_utils.h"

bool	count_rgb_parts(char **rgb_parts)
{
	int	i;

	i = 0;
	while (rgb_parts[i])
		i++;
	return (i == 3);
}

void	free_rgb_parts(char **rgb_parts)
{
	int	i;

	i = 0;
	while (rgb_parts[i])
	{
		free(rgb_parts[i]);
		i++;
	}
	free(rgb_parts);
}

bool	validate_rgb_values(int r, int g, int b)
{
	if (r < 0 || r > 255)
		return (false);
	if (g < 0 || g > 255)
		return (false);
	if (b < 0 || b > 255)
		return (false);
	return (true);
}

bool	parse_color_values(char **rgb_parts, int *r, int *g, int *b)
{
	*r = ft_atoi(rgb_parts[0]);
	*g = ft_atoi(rgb_parts[1]);
	*b = ft_atoi(rgb_parts[2]);
	return (validate_rgb_values(*r, *g, *b));
}

bool	parse_color(char *line, int *r, int *g, int *b)
{
	char	**rgb_parts;
	bool	result;

	rgb_parts = ft_split(line, ',');
	if (!rgb_parts)
		return (false);
	if (!count_rgb_parts(rgb_parts))
	{
		free_rgb_parts(rgb_parts);
		return (false);
	}
	result = parse_color_values(rgb_parts, r, g, b);
	free_rgb_parts(rgb_parts);
	return (result);
}
