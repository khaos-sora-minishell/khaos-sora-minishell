/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 21:12:42 by harici            #+#    #+#             */
/*   Updated: 2026/01/10 18:49:12 by harici           ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

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

char	*get_prompt(t_shell *shell)
{
	char	*prompt;
	char	*reset;

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
	if (isatty(STDIN_FILENO))
		next_line = readline(prompt);
	else
	{
		next_line = get_next_line(STDIN_FILENO);
		next_line = remove_trailing_newline(next_line);
	}
	return (next_line);
}
