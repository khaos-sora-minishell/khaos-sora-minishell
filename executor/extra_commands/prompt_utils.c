/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_utils_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 20:50:59 by akivam            #+#    #+#             */
/*   Updated: 2026/02/07 20:51:01 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "extra_commands.h"
#include "libft.h"

static void	restore_colors_from_env(t_shell *shell)
{
	char	*color;

	color = env_get(shell->env_table, "PROMPT_COLOR", shell->cmd_arena);
	if (color)
		shell->terminal_name_color = gc_strdup(shell->global_arena, color);
	color = env_get(shell->env_table, "PROMPT_BG_COLOR", shell->cmd_arena);
	if (color)
		shell->terminal_bg_color = gc_strdup(shell->global_arena, color);
}

static void	parse_name_from_ps1(t_shell *shell, char *current_ps1)
{
	char	*end;
	int		i;

	i = 0;
	while (current_ps1[i] && current_ps1[i] == '\033')
	{
		while (current_ps1[i] && current_ps1[i] != 'm')
			i++;
		if (current_ps1[i])
			i++;
	}
	if (!current_ps1[i])
		return ;
	end = ft_strchr(current_ps1 + i, '$');
	if (end && end > current_ps1 + i)
		shell->terminal_name = gc_strndup(shell->global_arena,
				current_ps1 + i, end - (current_ps1 + i));
}

static void	extract_terminal_name_from_ps1(t_shell *shell)
{
	char	*current_ps1;

	restore_colors_from_env(shell);
	current_ps1 = env_get(shell->env_table, "PS1", shell->cmd_arena);
	if (!current_ps1)
		return ;
	parse_name_from_ps1(shell, current_ps1);
}

void	update_ps1(t_shell *shell)
{
	char	*ps1;
	char	*temp;

	if (!shell->terminal_name && !shell->terminal_name_color
		&& !shell->terminal_bg_color)
		extract_terminal_name_from_ps1(shell);
	ps1 = gc_strdup(shell->global_arena, "");
	if (shell->terminal_bg_color)
		ps1 = gc_strjoin(shell->global_arena, ps1, shell->terminal_bg_color);
	if (shell->terminal_name_color)
		ps1 = gc_strjoin(shell->global_arena, ps1, shell->terminal_name_color);
	else if (shell->terminal_bg_color)
		ps1 = gc_strjoin(shell->global_arena, ps1, "\033[97m");
	if (shell->terminal_name)
		temp = shell->terminal_name;
	else
		temp = "minishell";
	ps1 = gc_strjoin(shell->global_arena, ps1, temp);
	ps1 = gc_strjoin(shell->global_arena, ps1, "\033[0m$ ");
	env_set(shell->env_table, "PS1", ps1, shell->global_arena);
}
