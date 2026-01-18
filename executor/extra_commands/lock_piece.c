/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lock_piece.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 19:36:28 by akivam            #+#    #+#             */
/*   Updated: 2026/01/18 19:36:28 by akivam           ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "extra_commands_bonus.h"

void	lock_piece(t_tetris *t)
{
	int	x;
	int	y;
	int	bx;
	int	by;

	y = 0;
	while (y < 4)
	{
		x = 0;
		while (x < 4)
		{
			if (t->pieces[t->current_piece][t->rotation][y][x])
			{
				bx = t->pos_x + x;
				by = t->pos_y + y;
				if (by >= 0 && by < t->board_h && bx >= 0 && bx < t->board_w)
					t->board[by][bx] = 1;
			}
			x++;
		}
		y++;
	}
	clear_lines(t);
	spawn_piece(t);
}
