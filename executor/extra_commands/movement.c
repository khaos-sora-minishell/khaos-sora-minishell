/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 19:36:28 by akivam            #+#    #+#             */
/*   Updated: 2026/01/18 19:36:28 by akivam           ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "extra_commands_bonus.h"

void	rotate_piece(t_tetris *t)
{
	int	new_rot;

	new_rot = (t->rotation + 1) % 4;
	if (!check_collision(t, t->pos_x, t->pos_y, new_rot))
		t->rotation = new_rot;
}

void	move_left(t_tetris *t)
{
	if (!check_collision(t, t->pos_x - 1, t->pos_y, t->rotation))
		t->pos_x--;
}

void	move_right(t_tetris *t)
{
	if (!check_collision(t, t->pos_x + 1, t->pos_y, t->rotation))
		t->pos_x++;
}

void	soft_drop(t_tetris *t)
{
	if (!check_collision(t, t->pos_x, t->pos_y + 1, t->rotation))
		t->pos_y++;
}

void	hard_drop(t_tetris *t)
{
	while (!check_collision(t, t->pos_x, t->pos_y + 1, t->rotation))
	{
		t->pos_y++;
		t->score += 2;
	}
	lock_piece(t);
}
