/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lock_piece.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 14:07:02 by harici            #+#    #+#             */
/*   Updated: 2026/02/07 14:07:02 by harici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tetris.h"

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
