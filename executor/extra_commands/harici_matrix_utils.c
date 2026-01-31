/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   harici_matrix_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 22:30:00 by akivam            #+#    #+#             */
/*   Updated: 2026/01/16 23:14:18 by harici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "extra_commands.h"
#include "libft.h"
#include <unistd.h>

unsigned int	simple_rand(unsigned int *seed)
{
	*seed = (*seed * 1103515245 + 12345) % 2147483648;
	return (*seed);
}

void	simple_delay(void)
{
	int	i;
	int	j;

	i = 0;
	while (i < 50000)
	{
		j = 0;
		while (j < 100)
			j++;
		i++;
	}
}

void	write_at(int row, int col, char c, int bright)
{
	char	buf[30];

	buf[0] = '\033';
	buf[1] = '[';
	buf[2] = '0' + (row / 10);
	buf[3] = '0' + (row % 10);
	buf[4] = ';';
	buf[5] = '0' + (col / 10);
	buf[6] = '0' + (col % 10);
	buf[7] = 'H';
	buf[8] = '\033';
	buf[9] = '[';
	if (bright)
		buf[10] = '1';
	else
		buf[10] = '2';
	buf[11] = ';';
	buf[12] = '3';
	buf[13] = '2';
	buf[14] = 'm';
	buf[15] = c;
	write(1, buf, 16);
}

void	draw_rain(int *drops, int width, int height, unsigned int *seed)
{
	int		x;
	char	c;

	x = 0;
	while (x < width)
	{
		if (drops[x] >= height)
			drops[x] = 0;
		c = "01"[simple_rand(seed) % 2];
		write_at(drops[x], x + 1, c, 1);
		if (drops[x] > 0)
			write_at(drops[x] - 1, x + 1, c, 0);
		drops[x]++;
		x++;
	}
}
