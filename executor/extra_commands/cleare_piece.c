/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleare_piece.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 20:37:28 by akivam            #+#    #+#             */
/*   Updated: 2026/01/18 20:37:33 by akivam           ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "extra_commands.h"

static void	shift_rows_down(t_tetris *t, int y)
{
	int	x;
	int	row;

	row = y;
	while (row > 0)
	{
		x = 0;
		while (x < t->board_w)
		{
			t->board[row][x] = t->board[row - 1][x];
			x++;
		}
		row--;
	}
	x = 0;
	while (x < t->board_w)
	{
		t->board[0][x] = 0;
		x++;
	}
}

static int	check_full_line(t_tetris *t, int y)
{
	int	x;

	x = 0;
	while (x < t->board_w)
	{
		if (t->board[y][x] == 0)
			return (0);
		x++;
	}
	return (1);
}

void	clear_lines(t_tetris *t)
{
	int	y;

	y = t->board_h - 1;
	while (y >= 0)
	{
		if (check_full_line(t, y))
		{
			shift_rows_down(t, y);
			t->lines++;
			t->score += 100;
			y++;
		}
		y--;
	}
}
