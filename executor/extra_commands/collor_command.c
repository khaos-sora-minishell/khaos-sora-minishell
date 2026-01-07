/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collor_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 19:49:05 by akivam            #+#    #+#             */
/*   Updated: 2025/12/22 19:49:35 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "extra_commands.h"
#include "libft.h"
#include "utils.h"

#ifdef BONUS

static char	*get_prompt_color(char *arg)
{
	if (ft_strcmp(arg, "red") == 0)
		return (C_RED);
	if (ft_strcmp(arg, "green") == 0)
		return (C_GREEN);
	if (ft_strcmp(arg, "yellow") == 0)
		return (C_YELLOW);
	if (ft_strcmp(arg, "blue") == 0)
		return (C_BLUE);
	if (ft_strcmp(arg, "magenta") == 0)
		return (C_MAGENTA);
	if (ft_strcmp(arg, "cyan") == 0)
		return (C_CYAN);
	if (ft_strcmp(arg, "white") == 0)
		return (C_WHITE);
	return (NULL);
}

static char	*get_background_color(char *arg)
{
	if (ft_strcmp(arg, "red") == 0)
		return (BG_RED);
	if (ft_strcmp(arg, "green") == 0)
		return (BG_GREEN);
	if (ft_strcmp(arg, "yellow") == 0)
		return (BG_YELLOW);
	if (ft_strcmp(arg, "blue") == 0)
		return (BG_BLUE);
	if (ft_strcmp(arg, "magenta") == 0)
		return (BG_MAGENTA);
	if (ft_strcmp(arg, "cyan") == 0)
		return (BG_CYAN);
	if (ft_strcmp(arg, "black") == 0)
		return (BG_BLACK);
	return (NULL);
}

static char	*get_color_code(char *arg, int is_background)
{
	if (!arg)
		return (NULL);
	if (ft_strcmp(arg, "reset") == 0)
		return (C_RESET);
	if (is_background)
		return (get_background_color(arg));
	else
		return (get_prompt_color(arg));
}

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
	shell->exit_status = 0;
}

void	set_background(char **args, t_shell *shell)
{
	char	*code;

	if (!args[1])
	{
		ft_printf("Usage: set-bg ");
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
	shell->exit_status = 0;
}

#endif
