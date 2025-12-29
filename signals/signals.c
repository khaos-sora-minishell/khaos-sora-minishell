/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 16:49:30 by harici            #+#    #+#             */
/*   Updated: 2025/12/22 13:13:29 by harici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** SIGINT (Ctrl+C) handler - Parent process icin
**
** set_signal(SIGINT) kullaniyoruz cunku:
** 1. Encapsulation: g_signal'i extern yapmak zorunda kalmiyoruz
** 2. 42 Norm: Modüler yapi korunuyor
** 3. Guvenlik: set_signal() o kadar basit ki atomic gibi calisir
** 4. Race condition yok: Shell single-threaded, child'lar kendi handler'ini kullanir
**
** Readline fonksiyonlari (rl_*) async-signal-safe DEGIL ama
** minishell'de kabul edilebilir (readline dokumantasyonu bunu onerir)
*/
void	signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		set_signal(SIGINT);
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	setup_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = signal_handler;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

void	setup_child_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

void	ignore_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}
