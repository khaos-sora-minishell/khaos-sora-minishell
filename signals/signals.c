/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 00:50:00 by harici            #+#    #+#             */
/*   Updated: 2025/12/12 00:50:01 by harici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Global signal variable definition
** (extern declaration is in minishell.h)
*/
volatile sig_atomic_t	g_signal = 0;

/*
** Signal handler for interactive mode
** SIGINT (Ctrl+C): Display new prompt on new line
*/
void	signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		g_signal = SIGINT;
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

/*
** Setup signals for interactive mode
** SIGINT (Ctrl+C): Handle with custom handler
** SIGQUIT (Ctrl+\): Ignore in interactive mode
*/
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

/*
** Restore default signal handlers for child processes
** SIGINT (Ctrl+C): Default behavior (terminate)
** SIGQUIT (Ctrl+\): Default behavior (quit with core dump)
*/
void	setup_child_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

/*
** Ignore both SIGINT and SIGQUIT
** Used during heredoc input and other special cases
*/
void	ignore_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}
