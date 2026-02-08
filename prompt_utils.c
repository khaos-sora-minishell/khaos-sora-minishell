/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 20:27:05 by harici            #+#    #+#             */
/*   Updated: 2026/01/13 20:27:05 by harici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"
#include "libft.h"
#include <readline/readline.h>
#include <stdio.h>
#include <unistd.h>

void	build_prompt_colors(t_shell *shell, char **prompt)
{
	if (shell->terminal_bg_color)
	{
		*prompt = gc_strjoin(shell->cmd_arena, *prompt, "\001");
		*prompt = gc_strjoin(shell->cmd_arena, *prompt,
				shell->terminal_bg_color);
		*prompt = gc_strjoin(shell->cmd_arena, *prompt, "\002");
	}
	if (shell->terminal_name_bg_color)
	{
		*prompt = gc_strjoin(shell->cmd_arena, *prompt, "\001");
		*prompt = gc_strjoin(shell->cmd_arena, *prompt,
				shell->terminal_name_bg_color);
		*prompt = gc_strjoin(shell->cmd_arena, *prompt, "\002");
	}
	if (shell->terminal_name_color)
	{
		*prompt = gc_strjoin(shell->cmd_arena, *prompt, "\001");
		*prompt = gc_strjoin(shell->cmd_arena, *prompt,
				shell->terminal_name_color);
		*prompt = gc_strjoin(shell->cmd_arena, *prompt, "\002");
	}
}

static char	*wrap_ansi_codes(char *ps1, t_shell *shell)
{
	char	*result;
	int		i;

	result = gc_strdup(shell->cmd_arena, "");
	i = 0;
	while (ps1[i])
	{
		if (ps1[i] == '\033')
		{
			result = gc_strjoin(shell->cmd_arena, result, "\001");
			while (ps1[i] && ps1[i] != 'm')
				result = gc_strjoin(shell->cmd_arena, result,
					gc_strndup(shell->cmd_arena, &ps1[i++], 1));
			if (ps1[i])
				result = gc_strjoin(shell->cmd_arena, result,
					gc_strndup(shell->cmd_arena, &ps1[i++], 1));
			result = gc_strjoin(shell->cmd_arena, result, "\002");
		}
		else
			result = gc_strjoin(shell->cmd_arena, result,
				gc_strndup(shell->cmd_arena, &ps1[i++], 1));
	}
	return (result);
}

static void	sync_from_ps1(t_shell *shell, char *ps1)
{
	int		i;
	char	*name_start;
	char	*name_end;

	i = 0;
	while (ps1[i] && ps1[i] == '\033')
	{
		while (ps1[i] && ps1[i] != 'm')
			i++;
		if (ps1[i])
			i++;
	}
	if (!ps1[i])
		return ;
	name_start = ps1 + i;
	name_end = name_start;
	while (*name_end && *name_end != '$' && *name_end != '>'
		&& *name_end != '\033')
		name_end++;
	if (name_end > name_start)
		shell->terminal_name = gc_strndup(shell->global_arena,
				name_start, name_end - name_start);
}

char	*get_prompt(t_shell *shell)
{
	char	*prompt;
	char	*ps1;
	char	*reset;

	ps1 = env_get(shell->env_table, "PS1", shell->cmd_arena);
	if (ps1)
	{
		sync_from_ps1(shell, ps1);
		return (wrap_ansi_codes(ps1, shell));
	}
	reset = "\001\033[0m\002";
	prompt = gc_strdup(shell->cmd_arena, "");
	build_prompt_colors(shell, &prompt);
	if (shell->terminal_name)
		prompt = gc_strjoin(shell->cmd_arena, prompt, shell->terminal_name);
	else
		prompt = gc_strjoin(shell->cmd_arena, prompt, "minishell");
	prompt = gc_strjoin(shell->cmd_arena, prompt, reset);
	prompt = gc_strjoin(shell->cmd_arena, prompt, "$ ");
	return (prompt);
}

char	*join_lines(char *input, char *next_line)
{
	char	*result;
	size_t	len1;
	size_t	len2;

	len1 = ft_strlen(input);
	len2 = ft_strlen(next_line);
	result = malloc(len1 + len2 + 2);
	if (!result)
		return (NULL);
	ft_memcpy(result, input, len1);
	result[len1] = '\n';
	ft_memcpy(result + len1 + 1, next_line, len2);
	result[len1 + len2 + 1] = '\0';
	return (result);
}

char	*remove_trailing_newline(char *str)
{
	size_t	len;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	if (len > 0 && str[len - 1] == '\n')
		str[len - 1] = '\0';
	return (str);
}

char	*get_continuation_line(char quote)
{
	char	*next_line;
	char	*prompt;

	prompt = "dquote> ";
	if (quote == '\'')
		prompt = "squote> ";
	if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
		next_line = readline(prompt);
	else
	{
		next_line = get_next_line(STDIN_FILENO);
		next_line = remove_trailing_newline(next_line);
	}
	return (next_line);
}
