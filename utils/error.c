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
#include "libft.h"

/*
 * Prints formatted error message to stderr
 * Format: minishell: cmd: arg: msg
 */
void	print_error(const char *cmd, const char *arg, const char *msg)
{
	if (arg)
		ft_err_printf("minishell: %s: %s: %s\n", cmd, arg, msg);
	else
		ft_err_printf("minishell: %s: %s\n", cmd, msg);
}

/*
 * Prints syntax error message for unexpected token
 */
void	syntax_error(char *token)
{
	ft_err_printf("minishell: syntax error near unexpected token `%s'\n",
		token);
}

/*
 * Prints command not found error and returns exit code 127
 */
int	command_not_found(char *cmd)
{
	ft_err_printf("minishell: %s: command not found\n", cmd);
	return (127);
}

/*
 * Prints error message and exits shell with status 1
 */
void	error_exit(char *msg, t_shell *shell)
{
	(void)shell;
	ft_err_printf("minishell: %s\n", msg);
	exit(1);
}
