/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 14:06:19 by harici            #+#    #+#             */
/*   Updated: 2026/02/07 18:05:09 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tetris.h"

void	append_string(char *buffer, int *pos, const char *str)
{
	while (*str && *pos < TETRIS_BUFFER_SIZE - 1)
		buffer[(*pos)++] = *str++;
	buffer[*pos] = '\0';
}

void	append_int(char *buffer, int *pos, int n)
{
	char	temp[16];
	int		i;

	if (n == 0)
	{
		append_string(buffer, pos, "0");
		return ;
	}
	i = 0;
	while (n > 0)
	{
		temp[i++] = (n % 10) + '0';
		n /= 10;
	}
	while (i > 0)
	{
		if (*pos < TETRIS_BUFFER_SIZE - 1)
			buffer[(*pos)++] = temp[--i];
		else
			break ;
	}
}
