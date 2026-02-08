/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 14:07:07 by harici            #+#    #+#             */
/*   Updated: 2026/02/07 14:07:08 by harici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tetris.h"

static void	handle_escape_sequence(t_tetris *t, char code)
{
	if (t->paused)
	{
		if (code == 'A')
			t->menu_selection = 0;
		else if (code == 'B')
			t->menu_selection = 1;
	}
	else
	{
		if (code == 'A')
			rotate_piece(t);
		else if (code == 'B')
			soft_drop(t);
		else if (code == 'C')
			move_right(t);
		else if (code == 'D')
			move_left(t);
	}
}

static void	handle_escape(t_tetris *t)
{
	char	seq[2];

	if (read(STDIN_FILENO, &seq[0], 1) != 1)
	{
		if (t->paused)
			t->paused = 0;
		else
			t->paused = 1;
		return ;
	}
	if (seq[0] != '[')
		return ;
	if (read(STDIN_FILENO, &seq[1], 1) != 1)
		return ;
	handle_escape_sequence(t, seq[1]);
}

static void	handle_menu_input(t_tetris *t, char c)
{
	if (c == 'w')
		t->menu_selection = 0;
	else if (c == 's')
		t->menu_selection = 1;
	else if (c == '\n' || c == ' ')
	{
		if (t->menu_selection == 0)
			t->paused = 0;
		else if (t->menu_selection == 1)
			t->running = 0;
	}
}

static void	handle_gameplay_input(t_tetris *t, char c)
{
	if (c == KEY_ROTATE)
		rotate_piece(t);
	else if (c == KEY_SOFT_DROP)
		soft_drop(t);
	else if (c == KEY_LEFT)
		move_left(t);
	else if (c == KEY_RIGHT)
		move_right(t);
	else if (c == KEY_HARD_DROP)
		hard_drop(t);
}

void	handle_input(t_shell *shell)
{
	t_tetris	*t;
	char		c;

	t = shell->tetris;
	while (read(STDIN_FILENO, &c, 1) == 1)
	{
		if (c == KEY_CTRL_C || c == KEY_CTRL_D || c == KEY_QUIT)
		{
			t->running = 0;
			return ;
		}
		if (t->game_over)
		{
			if (c == KEY_ESC || c == '\n' || c == ' ')
				t->running = 0;
			return ;
		}
		if (c == KEY_ESC)
			handle_escape(t);
		else if (t->paused)
			handle_menu_input(t, c);
		else
			handle_gameplay_input(t, c);
	}
}
