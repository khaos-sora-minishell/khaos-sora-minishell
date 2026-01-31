/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spawn_piece.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 19:36:28 by akivam            #+#    #+#             */
/*   Updated: 2026/01/18 19:36:28 by akivam           ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "extra_commands.h"

void	spawn_piece(t_tetris *t)
{
	unsigned int	r;

	r = simple_rand(&t->random_number_seed);
	t->current_piece = r % 7;
	t->rotation = 0;
	t->pos_x = t->board_w / 2 - 2;
	t->pos_y = 0;
	if (check_collision(t, t->pos_x, t->pos_y, t->rotation))
		t->running = 0;
}
