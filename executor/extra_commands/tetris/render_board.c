/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_board.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 14:06:30 by harici            #+#    #+#             */
/*   Updated: 2026/02/07 14:06:31 by harici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tetris.h"

static int	check_active_piece(t_tetris *t, int x, int y)
{
	int	px;
	int	py;

	py = y - t->pos_y;
	px = x - t->pos_x;
	if (px >= 0 && px < 4 && py >= 0 && py < 4)
	{
		if (t->pieces[t->current_piece][t->rotation][py][px])
			return (1);
	}
	return (0);
}

int	get_ghost_y(t_tetris *t)
{
	int	ghost_y;

	ghost_y = t->pos_y;
	while (!check_collision(t, t->pos_x, ghost_y + 1, t->rotation))
		ghost_y++;
	return (ghost_y);
}

static int	check_ghost_piece(t_tetris *t, int x, int y)
{
	int	px;
	int	py;

	py = y - t->ghost_y;
	px = x - t->pos_x;
	if (px >= 0 && px < 4 && py >= 0 && py < 4)
	{
		if (t->pieces[t->current_piece][t->rotation][py][px])
			return (1);
	}
	return (0);
}

static void	render_paused_menu(t_tetris *t, char *buffer, int *pos, int y)
{
	if (y == 8)
		append_string(buffer, pos, "       PAUSED       ");
	else if (y == 10)
	{
		if (t->menu_selection == 0)
			append_string(buffer, pos, "      > Return      ");
		else
			append_string(buffer, pos, "        Return      ");
	}
	else if (y == 11)
	{
		if (t->menu_selection == 1)
			append_string(buffer, pos, "      > Exit        ");
		else
			append_string(buffer, pos, "        Exit        ");
	}
	else
		append_string(buffer, pos, "                    ");
}

void	render_board_line(t_tetris *t, char *buffer, int *pos, int y)
{
	int	x;

	if (t->paused && (y == 8 || y == 9 || y == 10 || y == 11 || y == 12))
	{
		render_paused_menu(t, buffer, pos, y);
		return ;
	}
	x = 0;
	while (x < t->board_w)
	{
		if (t->board[y][x])
			append_string(buffer, pos, "[]");
		else if (check_active_piece(t, x, y))
			append_string(buffer, pos, "[]");
		else if (check_ghost_piece(t, x, y))
			append_string(buffer, pos, "::");
		else
			append_string(buffer, pos, "  ");
		x++;
	}
}
