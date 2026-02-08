/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tetris.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 14:05:51 by harici            #+#    #+#             */
/*   Updated: 2026/02/07 18:01:26 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tetris.h"

void	update_game(t_shell *shell)
{
	t_tetris	*t;

	t = shell->tetris;
	if (t->paused || t->game_over)
		return ;
	t->tick++;
	t->speed = calc_speed(t->level);
	if (t->tick >= t->speed)
	{
		if (!check_collision(t, t->pos_x, t->pos_y + 1, t->rotation))
			t->pos_y++;
		else
			lock_piece(t);
		t->tick = 0;
	}
}

static void	game_loop(t_shell *shell)
{
	while (shell->tetris->running)
	{
		handle_input(shell);
		update_game(shell);
		render(shell->tetris);
		simple_delay();
	}
}

void	start_game(char **args, t_shell *shell)
{
	struct termios	old;

	(void)args;
	setup_tetris_terminal(&old);
	write(1, CLEAR, 4);
	shell->tetris = gc_malloc(shell->cmd_arena, sizeof(t_tetris));
	if (!shell->tetris)
		return ;
	init_tetris(shell->tetris, shell->cmd_arena);
	game_loop(shell);
	tcsetattr(STDIN_FILENO, TCSANOW, &old);
	write(1, "\033[?25h", 6);
}
