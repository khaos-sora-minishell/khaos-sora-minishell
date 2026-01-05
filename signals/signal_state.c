/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_state.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 13:15:00 by harici            #+#    #+#             */
/*   Updated: 2025/12/22 13:15:00 by harici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static volatile sig_atomic_t	g_signal = 0;

int	get_signal(void)
{
	return (g_signal);
}

void	set_signal(int value)
{
	g_signal = value;
}

void	reset_signal(void)
{
	g_signal = 0;
}
