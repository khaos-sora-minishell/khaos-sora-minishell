/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_error.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 21:42:42 by akivam            #+#    #+#             */
/*   Updated: 2025/12/22 15:30:00 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor_error.h"
#include "libft.h"
#include "minishell.h"
#include <stdlib.h>
#include <unistd.h>

void	print_execution_error(char *cmd, char *msg)
{
	ft_putstr_fd("minishell: ", 2);
	if (cmd)
	{
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putendl_fd(msg, 2);
}

void	exit_with_error(char *cmd, char *msg, int code, t_shell *shell)
{
	print_execution_error(cmd, msg);
	if (shell)
		cleanup_shell(shell);
	exit(code);
}

void	handle_cmd_not_found(char *cmd, t_shell *shell)
{
	exit_with_error(cmd, ERR_CMD_NOT_FOUND, EXIT_CMD_NOT_FOUND, shell);
}

void	handle_permission_denied(char *cmd, t_shell *shell)
{
	exit_with_error(cmd, ERR_PERM_DENIED, EXIT_CANNOT_EXEC, shell);
}

void	handle_is_directory(char *cmd, t_shell *shell)
{
	exit_with_error(cmd, ERR_IS_DIR, EXIT_CANNOT_EXEC, shell);
}

void	handle_no_such_file(char *cmd, t_shell *shell)
{
	exit_with_error(cmd, ERR_NO_FILE, EXIT_CMD_NOT_FOUND, shell);
}

