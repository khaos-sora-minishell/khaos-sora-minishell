/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_error.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 21:42:42 by akivam            #+#    #+#             */
/*   Updated: 2025/12/21 21:17:23 by akivam           ###   ########.fr       */
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

void	handle_cmd_not_found(char *cmd)
{
	print_execution_error(cmd, ERR_CMD_NOT_FOUND);
	exit(EXIT_CMD_NOT_FOUND);
}

void	handle_permission_denied(char *cmd)
{
	print_execution_error(cmd, ERR_PERM_DENIED);
	exit(EXIT_CANNOT_EXEC);
}

void	handle_is_directory(char *cmd)
{
	print_execution_error(cmd, ERR_IS_DIR);
	exit(EXIT_CANNOT_EXEC);
}
