/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collor_command_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 20:50:34 by akivam            #+#    #+#             */
/*   Updated: 2026/02/07 20:50:34 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "extra_commands_bonus.h"
#include "libft.h"

void	set_prompt(char **args, t_shell *shell)
{
	char	*code;

	if (!args[1])
	{
		ft_printf("Usage: set-prompt ");
		ft_printf("[red/green/blue/cyan/yellow/white/reset]\n");
		shell->exit_status = 1;
		return ;
	}
	code = get_color_code(args[1], 0);
	if (!code)
	{
		ft_printf("Error: '%s' unknown color.\n", args[1]);
		shell->exit_status = 1;
		return ;
	}
	shell->terminal_name_color = gc_strdup(shell->global_arena, code);
	env_set(shell->env_table, "PROMPT_COLOR", code, shell->global_arena);
	update_ps1(shell);
	shell->exit_status = 0;
}

void	set_background(char **args, t_shell *shell)
{
	char	*code;

	if (!args[1])
	{
		ft_printf("Usage: set-background ");
		ft_printf("[red/green/blue/cyan/yellow/black/reset]\n");
		shell->exit_status = 1;
		return ;
	}
	code = get_color_code(args[1], 1);
	if (!code)
	{
		ft_printf("Error: '%s' unknown background color.\n", args[1]);
		shell->exit_status = 1;
		return ;
	}
	shell->terminal_bg_color = gc_strdup(shell->global_arena, code);
	env_set(shell->env_table, "PROMPT_BG_COLOR", code, shell->global_arena);
	update_ps1(shell);
	shell->exit_status = 0;
}
