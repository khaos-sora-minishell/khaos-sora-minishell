/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tetromino.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 14:05:25 by harici            #+#    #+#             */
/*   Updated: 2026/02/07 14:05:26 by harici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tetris.h"

static void	fill_shape(int shape[4][4], unsigned int mask)
{
	int	y;
	int	x;

	y = 0;
	while (y < 4)
	{
		x = 0;
		while (x < 4)
		{
			shape[y][x] = (mask >> ((3 - y) * 4 + (3 - x))) & 1;
			x++;
		}
		y++;
	}
}

const t_shape	*get_tetromino_shapes(void)
{
	static int			shapes[7][4][4][4];
	static int			init = 0;
	int					i;
	static unsigned int	m[] = {
		0x0F00, 0x2222, 0x00F0, 0x4444, 0x8E00, 0x6440, 0x0E20, 0x44C0,
		0x2E00, 0x4460, 0x0E80, 0xC440, 0x6600, 0x6600, 0x6600, 0x6600,
		0x6C00, 0x4620, 0x06C0, 0x8C40, 0x4E00, 0x4640, 0x0E40, 0x4C40,
		0xC600, 0x2640, 0x0C60, 0x4C80
	};

	if (init)
		return ((const t_shape *)shapes);
	i = -1;
	while (++i < 28)
		fill_shape(shapes[i / 4][i % 4], m[i]);
	init = 1;
	return ((const t_shape *)shapes);
}
