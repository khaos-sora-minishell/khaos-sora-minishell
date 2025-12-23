/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 02:30:00 by harici            #+#    #+#             */
/*   Updated: 2025/12/24 02:06:46 by harici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"
#include "libft.h"
#include <readline/readline.h>
#include <stdio.h>

static void	build_prompt_colors(t_shell *shell, char **prompt)
{
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

static char	*join_lines(char *input, char *next_line)
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

static char	*read_continuation(char *input, char quote, t_shell *shell)
{
	char	*next_line;
	char	*result;
	char	*prompt;

	prompt = "dquote> ";
	if (quote == '\'')
		prompt = "squote> ";
	next_line = readline(prompt);
	if (!next_line)
	{
		ft_putstr_fd("minishell: unexpected EOF while looking for "
			"matching `", 2);
		ft_putchar_fd(quote, 2);
		ft_putendl_fd("'", 2);
		shell->exit_status = 2;
		free(input);
		return (NULL);
	}
	result = join_lines(input, next_line);
	free(input);
	free(next_line);
	return (result);
}

char	*read_multiline(t_shell *shell)
{
	char	*input;
	char	quote;

	input = readline(get_prompt(shell));
	if (!input)
		return (NULL);
	quote = check_unclosed_quotes_char(input);
	while (quote)
	{
		input = read_continuation(input, quote, shell);
		if (!input)
			return (NULL);
		quote = check_unclosed_quotes_char(input);
	}
	return (input);
}
