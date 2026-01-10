/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 21:11:14 by harici            #+#    #+#             */
/*   Updated: 2026/01/10 18:50:15 by harici           ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

/*
 * Handles SIGINT (Ctrl+C) in interactive mode
 * Displays new prompt line without exiting shell
 */
void signal_handler(int signum) {
  if (signum == SIGINT) {
    set_signal(SIGINT);
    write(1, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
  }
}

/*
 * Configures signal handlers for parent shell process
 * SIGINT handled by custom handler, SIGQUIT and SIGPIPE ignored
 */
void setup_signals(void) {
  struct sigaction sa_int;
  struct sigaction sa_quit;
  struct sigaction sa_pipe;

  sa_int.sa_handler = signal_handler;
  sigemptyset(&sa_int.sa_mask);
  sa_int.sa_flags = SA_RESTART;
  sigaction(SIGINT, &sa_int, NULL);
  sa_quit.sa_handler = SIG_IGN;
  sigemptyset(&sa_quit.sa_mask);
  sa_quit.sa_flags = 0;
  sigaction(SIGQUIT, &sa_quit, NULL);
  sa_pipe.sa_handler = SIG_IGN;
  sigemptyset(&sa_pipe.sa_mask);
  sa_pipe.sa_flags = 0;
  sigaction(SIGPIPE, &sa_pipe, NULL);
}

/*
 * Restores default signal behavior for child processes
 * SIGINT and SIGQUIT reset to default, SIGPIPE ignored
 */
void setup_child_signals(void) {
  struct sigaction sa;

  sa.sa_handler = SIG_DFL;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sigaction(SIGINT, &sa, NULL);
  sigaction(SIGQUIT, &sa, NULL);
  sa.sa_handler = SIG_IGN;
  sigaction(SIGPIPE, &sa, NULL);
}

/*
 * Ignores SIGINT and SIGQUIT signals
 * Used during child process execution to prevent interruption
 */
void ignore_signals(void) {
  struct sigaction sa;

  sa.sa_handler = SIG_IGN;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sigaction(SIGINT, &sa, NULL);
  sigaction(SIGQUIT, &sa, NULL);
}
