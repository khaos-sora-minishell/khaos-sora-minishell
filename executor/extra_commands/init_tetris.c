/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_tetris.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 19:36:28 by akivam            #+#    #+#             */
/*   Updated: 2026/01/18 19:36:28 by akivam           ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "extra_commands_bonus.h"

static int	**allocate_board(int h, int w, void *arena)
{
	int	**board;
	int	i;
	int	j;

	board = (int **)gc_malloc(arena, sizeof(int *) * h);
	i = 0;
	while (i < h)
	{
		board[i] = (int *)gc_malloc(arena, sizeof(int) * w);
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
	t->random_number_seed = 42;
	t->running = 1;
	spawn_piece(t);
}
