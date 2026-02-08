/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calc_speed.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 14:07:28 by harici            #+#    #+#             */
/*   Updated: 2026/02/07 14:07:28 by harici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tetris.h"

int	calc_speed(int level)
{
	int	base;

	base = 10 - level;
	if (base < 2)
		base = 2;
	return (base);
}
