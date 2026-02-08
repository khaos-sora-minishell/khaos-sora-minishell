/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_terminal_name_bonus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 21:47:22 by akivam            #+#    #+#             */
/*   Updated: 2026/01/16 23:15:32 by harici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "extra_commands.h"
#include "garbage_collector.h"
#include "libft.h"
#include "minishell.h"

void	set_terminal_name(char **args, t_shell *shell)
{
	if (!args[1])
	{
		ft_err_printf("Error: argument is missing\n");
		ft_err_printf("Usage: set-terminal <name>\n");
		shell->exit_status = 1;
		return ;
	}
	shell->terminal_name = gc_strdup(shell->global_arena, args[1]);
	update_ps1(shell);
	shell->exit_status = 0;
}
