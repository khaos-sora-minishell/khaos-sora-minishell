/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 16:52:54 by harici            #+#    #+#             */
/*   Updated: 2025/12/21 21:36:09 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

/*
 * Prints formatted error message to stderr
 * Format: minishell: cmd: arg: msg
 */
void	print_error(const char *cmd, const char *arg, const char *msg)
{
	fprintf(stderr, "minishell: %s", cmd);
	if (arg)
		fprintf(stderr, ": %s", arg);
	fprintf(stderr, ": %s\n", msg);
}

/*
 * Prints syntax error message for unexpected token
 */
void	syntax_error(char *token)
{
	fprintf(stderr, "minishell: syntax error near unexpected token `%s'\n",
		token);
}

/*
 * Prints command not found error and returns exit code 127
 */
int	command_not_found(char *cmd)
{
	fprintf(stderr, "minishell: %s: command not found\n", cmd);
	return (127);
}

/*
 * Prints error message and exits shell with status 1
 */
void	error_exit(char *msg, t_shell *shell)
{
	(void)shell;
	fprintf(stderr, "minishell: %s\n", msg);
	exit(1);
}
