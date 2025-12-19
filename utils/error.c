/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 16:52:54 by harici            #+#    #+#             */
/*   Updated: 2025/12/19 16:52:55 by harici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

/*
 * Print error message
 */
void	print_error(const char *cmd, const char *arg, const char *msg)
{
	/*! bizim printflere geçilecek*/
	fprintf(stderr, "minishell: %s", cmd);
	if (arg)
		fprintf(stderr, ": %s", arg);
	fprintf(stderr, ": %s\n", msg);
}

/*
 * Print syntax error
 */
void	syntax_error(char *token)
{
	fprintf(stderr, "minishell: syntax error near unexpected token `%s'\n", token);
}

/*
 * Print command not found error
 */
int	command_not_found(char *cmd)
{
	fprintf(stderr, "minishell: %s: command not found\n", cmd);
	return (127);
}

/*
 * Print error and exit
 */
void	error_exit(char *msg, t_shell *shell)
{
	(void)shell;
	fprintf(stderr, "minishell: %s\n", msg);
	exit(1);
}
