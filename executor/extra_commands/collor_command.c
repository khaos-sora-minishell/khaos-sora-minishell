/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collor_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 20:50:34 by akivam            #+#    #+#             */
/*   Updated: 2026/02/09 02:23:20 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "extra_commands.h"
#include "libft.h"
#include "utils.h"

static void	handle_prompt_reset(t_shell *shell)
{
	shell->terminal_name_color = NULL;
	env_set(shell->env_table, "PROMPT_COLOR", "", shell->global_arena);
	update_ps1(shell);
	shell->exit_status = 0;
}

static void	apply_prompt_color(char **args, t_shell *shell)
{
	char	*code;

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

void	set_prompt(char **args, t_shell *shell)
{
	if (!args[1])
	{
		ft_printf("Usage: set-prompt ");
		ft_printf("[red/green/blue/cyan/yellow/white/reset]\n");
		shell->exit_status = 1;
		return ;
	}
	if (ft_strcmp(args[1], "reset") == 0)
	{
		handle_prompt_reset(shell);
		return ;
	}
	apply_prompt_color(args, shell);
}

static void	apply_background_color(char **args, t_shell *shell)
{
	char	*code;

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

void	set_background(char **args, t_shell *shell)
{
	if (!args[1])
	{
		ft_printf("Usage: set-background ");
		ft_printf("[red/green/blue/cyan/yellow/black/reset]\n");
		shell->exit_status = 1;
		return ;
	}
	if (ft_strcmp(args[1], "reset") == 0)
	{
		shell->terminal_bg_color = NULL;
		env_set(shell->env_table, "PROMPT_BG_COLOR", "", shell->global_arena);
		update_ps1(shell);
		shell->exit_status = 0;
		return ;
	}
	apply_background_color(args, shell);
}
