/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spawn_piece.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 14:06:04 by harici            #+#    #+#             */
/*   Updated: 2026/02/07 14:06:04 by harici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tetris.h"

void	spawn_piece(t_tetris *t)
{
	t->current_piece = t->next_piece;
	t->next_piece = xorshift32(&t->random_number_seed) % 7;
	t->rotation = 0;
	t->pos_x = t->board_w / 2 - 2;
	t->pos_y = 0;
	if (check_collision(t, t->pos_x, t->pos_y, t->rotation))
		t->game_over = 1;
}
