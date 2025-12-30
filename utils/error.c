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

void	print_error(const char *cmd, const char *arg, const char *msg)
{
	fprintf(stderr, "minishell: %s", cmd);
	if (arg)
		fprintf(stderr, ": %s", arg);
	fprintf(stderr, ": %s\n", msg);
}

void	syntax_error(char *token)
{
	fprintf(stderr, "minishell: syntax error near unexpected token `%s'\n",
		token);
}

int	command_not_found(char *cmd)
{
	fprintf(stderr, "minishell: %s: command not found\n", cmd);
	return (127);
}

void	error_exit(char *msg, t_shell *shell)
{
	(void)shell;
	fprintf(stderr, "minishell: %s\n", msg);
	exit(1);
}
