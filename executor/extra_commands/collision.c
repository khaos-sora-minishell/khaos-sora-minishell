/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collision.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 20:38:16 by akivam            #+#    #+#             */
/*   Updated: 2026/01/18 20:38:19 by akivam           ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "extra_commands_bonus.h"

int	check_collision(t_tetris *t, int new_x, int new_y, int new_rot)
{
	int	x;
	int	y;
	int	bx;
	int	by;

	y = -1;
	while (++y < 4)
	{
		x = -1;
		while (++x < 4)
		{
			if (t->pieces[t->current_piece][new_rot][y][x])
			{
				bx = new_x + x;
				by = new_y + y;
				if (bx < 0 || bx >= t->board_w)
					return (1);
				if (by >= t->board_h)
					return (1);
				if (by >= 0 && t->board[by][bx])
					return (1);
			}
		}
	}
	return (0);
}
