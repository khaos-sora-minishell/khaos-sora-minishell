/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 00:00:21 by harici            #+#    #+#             */
/*   Updated: 2025/12/24 01:56:54 by harici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"

char	check_unclosed_quotes_char(char *input)
{
	int		i;
	char	quote;

	i = 0;
	quote = 0;
	while (input[i])
	{
		if (!quote && (input[i] == '\'' || input[i] == '"'))
			quote = input[i];
		else if (quote && input[i] == quote)
			quote = 0;
		i++;
	}
	return (quote);
}

static t_token	*parse_alias_value(char *alias_value, t_shell *shell)
{
	t_token	*token_list;
	t_token	*new_token;
	char	**words;
	int		i;

	if (!alias_value || !*alias_value)
		return (NULL);
	words = ft_split(alias_value, ' ');
	if (!words)
		return (NULL);
	token_list = NULL;
	i = 0;
	while (words[i])
	{
		new_token = create_token(TOKEN_WORD,
				gc_strdup(shell->cmd_arena, words[i]), shell->cmd_arena);
		if (new_token)
			add_token(&token_list, new_token);
		i++;
	}
	free(words);
	return (token_list);
}

static t_token	*append_tokens(t_token *alias_tokens, t_token *user_tokens)
{
	t_token	*current;

	if (!alias_tokens)
		return (user_tokens);
	current = alias_tokens;
	while (current->next)
		current = current->next;
	current->next = user_tokens;
	return (alias_tokens);
}

static t_token	*expand_alias_if_needed(t_token *tokens, t_shell *shell)
{
	char	*alias_value;
	t_token	*alias_tokens;

	if (!tokens || tokens->type != TOKEN_WORD || !shell->alias_table)
		return (tokens);
	alias_value = env_get(shell->alias_table, tokens->value, shell->cmd_arena);
	if (!alias_value)
		return (tokens);
	alias_tokens = parse_alias_value(alias_value, shell);
	if (!alias_tokens)
		return (tokens);
	return (append_tokens(alias_tokens, tokens->next));
}

static int	check_unclosed_quotes(char *input, t_shell *shell)
{
	char	quote;

	quote = check_unclosed_quotes_char(input);
	if (quote)
	{
		ft_putstr_fd("minishell: unexpected EOF while looking for matching `",
			2);
		ft_putchar_fd(quote, 2);
		ft_putendl_fd("'", 2);
		shell->exit_status = 2;
		return (1);
	}
	return (0);
}

static void	process_operator(char *input, int *i, t_token **tokens, t_shell *sh)
{
	t_token_type	op_type;

	op_type = handle_operator(input, i);
	add_token(tokens, create_token(op_type,
			get_operator_value(op_type), sh->cmd_arena));
}

static void	process_word(char *input, int *i, t_token **tokens, t_shell *shell)
{
	char	*word;

	word = extract_word(input, i, shell);
	if (word)
		add_token(tokens, create_token(TOKEN_WORD, word, shell->cmd_arena));
}

t_token	*lexer(char *input, t_shell *shell)
{
	t_token	*tokens;
	int		i;

	if (!input || !shell)
		return (NULL);
	if (check_unclosed_quotes(input, shell))
		return (NULL);
	tokens = NULL;
	i = 0;
	while (input[i])
	{
		while (input[i] && is_whitespace(input[i]))
			i++;
		if (!input[i])
			break ;
		if (is_special_char(input[i]))
			process_operator(input, &i, &tokens, shell);
		else
			process_word(input, &i, &tokens, shell);
	}
	tokens = expand_alias_if_needed(tokens, shell);
	return (tokens);
}
