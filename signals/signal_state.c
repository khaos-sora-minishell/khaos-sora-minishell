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

/*
** Signal state yonetimi - Encapsulation ile yapilmistir
**
** volatile sig_atomic_t: Atomic okuma/yazma garantisi verir
** - volatile: Compiler optimization'lari engeller
** - sig_atomic_t: Signal handler'da guvenli kullanim
** - static: Sadece bu modülden erisim (42 norm uyumlu)
*/
static volatile sig_atomic_t	g_signal = 0;

int	get_signal(void)
{
	return (g_signal);
}

/*
** Signal handler'dan cagrilir
** NOT: POSIX'e gore signal handler'da fonksiyon cagrisi risklidir,
** AMA bu fonksiyon O KADAR basit ki (tek satirlik atama) guvenlidir:
** - Compiler muhtemelen inline yapacak
** - Gercek islem atomic (sig_atomic_t sayesinde)
** - 42 norm icin encapsulation korur
*/
void	set_signal(int value)
{
	g_signal = value;
}

void	reset_signal(void)
{
	g_signal = 0;
}
