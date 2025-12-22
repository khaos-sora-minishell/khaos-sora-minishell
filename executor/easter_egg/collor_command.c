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
#include <stdio.h>

static char	*get_color_code(char *arg, int is_bg)
{
	if (!arg)
		return (NULL);
	if (ft_strcmp(arg, "reset") == 0)
		return (RESET);
	if (is_bg) // Arka Plan Kodları (set-bg)
	{
		if (ft_strcmp(arg, "red") == 0)
			return (B_RED);
		if (ft_strcmp(arg, "green") == 0)
			return (B_GREEN);
		if (ft_strcmp(arg, "yellow") == 0)
			return (B_YELLOW);
		if (ft_strcmp(arg, "blue") == 0)
			return (B_BLUE);
		if (ft_strcmp(arg, "magenta") == 0)
			return (B_MAGENTA);
		if (ft_strcmp(arg, "cyan") == 0)
			return (B_CYAN);
		if (ft_strcmp(arg, "black") == 0)
			return (B_BLACK);
	}
	else // Prompt Renkleri (set-prompt)
	{
		if (ft_strcmp(arg, "red") == 0)
			return (F_RED);
		if (ft_strcmp(arg, "green") == 0)
			return (F_GREEN);
		if (ft_strcmp(arg, "yellow") == 0)
			return (F_YELLOW);
		if (ft_strcmp(arg, "blue") == 0)
			return (F_BLUE);
		if (ft_strcmp(arg, "magenta") == 0)
			return (F_MAGENTA);
		if (ft_strcmp(arg, "cyan") == 0)
			return (F_CYAN);
		if (ft_strcmp(arg, "white") == 0)
			return (F_WHITE);
	}
	return (NULL);
}

int	builtin_set_prompt(char **args, t_shell *shell)
{
	char	*code;

	if (!args[1])
	{
		printf("Kullanim: set-prompt [red/green/blue/cyan/yellow/white/reset]\n");
		return (1);
	}
	code = get_color_code(args[1], 0);
	if (!code)
	{
		printf("Hata: '%s' bilinmeyen bir renk.\n", args[1]);
		return (1);
	}
	shell->terminal_name_color = gc_strdup(shell->global_arena, code);
	return (0);
}

int	builtin_set_bg(char **args, t_shell *shell)
{
	char	*code;

	if (!args[1])
	{
		printf("Kullanim: set-bg [red/green/blue/cyan/yellow/black/reset]\n");
		return (1);
	}
	code = get_color_code(args[1], 1);
	if (!code)
	{
		printf("Hata: '%s' bilinmeyen bir arka plan rengi.\n", args[1]);
		return (1);
	}
	shell->terminal_bg_color = gc_strdup(shell->global_arena, code);
	return (0);
}
