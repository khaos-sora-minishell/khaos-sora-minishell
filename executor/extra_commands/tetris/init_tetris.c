/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_tetris.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 14:07:12 by harici            #+#    #+#             */
/*   Updated: 2026/02/07 18:01:25 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tetris.h"

static int	**allocate_board(int h, int w, void *arena)
{
	int	**board;
	int	i;
	int	j;

	board = (int **)gc_malloc(arena, sizeof(int *) * h);
	if (!board)
		return (NULL);
	i = 0;
	while (i < h)
	{
		board[i] = (int *)gc_malloc(arena, sizeof(int) * w);
		if (!board[i])
			return (NULL);
		j = 0;
		while (j < w)
		{
			board[i][j] = 0;
			j++;
		}
		i++;
	}
	return (board);
}

void	init_tetris(t_tetris *t, void *arena)
{
	t->board_w = 10;
	t->board_h = 20;
	t->board = allocate_board(t->board_h, t->board_w, arena);
	t->pieces = get_tetromino_shapes();
	t->score = 0;
	t->level = 1;
	t->lines = 0;
	t->tick = 0;
	t->speed = calc_speed(t->level);
	t->random_number_seed = get_random_seed();
	t->running = 1;
	t->paused = 0;
	t->menu_selection = 0;
	t->game_over = 0;
	t->next_piece = xorshift32(&t->random_number_seed) % 7;
	spawn_piece(t);
}
