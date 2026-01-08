/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_state.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 21:08:28 by harici            #+#    #+#             */
/*   Updated: 2026/01/05 21:08:28 by harici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static volatile sig_atomic_t	g_signal = 0;

/*
 * Returns the current signal state
 */
int	get_signal(void)
{
	return (g_signal);
}

/*
 * Sets the global signal state to the specified value
 */
void	set_signal(int value)
{
	g_signal = value;
}

/*
 * Resets the global signal state to zero
 */
void	reset_signal(void)
{
	g_signal = 0;
}
