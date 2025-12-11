/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 00:00:21 by harici            #+#    #+#             */
/*   Updated: 2025/12/12 00:00:25 by harici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*create_token(t_token_type type, char *value, void *arena)
{
	t_token	*token;

	token = gc_malloc(arena, sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = gc_strdup(arena, value);
	token->next = NULL;
	return (token);
}

void	add_token(t_token **head, t_token *new_token)
{
	t_token	*current;

	if (!*head)
	{
		*head = new_token;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_token;
}

static t_token_type	handle_operator(char *input, int *i)
{
	char	c;
	char	next;

	c = input[*i];
	next = input[*i + 1];
	if (c == '|' && next == '|')
	{
		(*i) += 2;
		return (TOKEN_OR);
	}
	else if (c == '|')
	{
		(*i)++;
		return (TOKEN_PIPE);
	}
	else if (c == '&' && next == '&')
	{
		(*i) += 2;
		return (TOKEN_AND);
	}
	else if (c == '<' && next == '<')
	{
		(*i) += 2;
		return (TOKEN_HEREDOC);
	}
	else if (c == '<')
	{
		(*i)++;
		return (TOKEN_REDIR_IN);
	}
	else if (c == '>' && next == '>')
	{
		(*i) += 2;
		return (TOKEN_REDIR_APPEND);
	}
	else if (c == '>')
	{
		(*i)++;
		return (TOKEN_REDIR_OUT);
	}
	else if (c == '(')
	{
		(*i)++;
		return (TOKEN_LPAREN);
	}
	else if (c == ')')
	{
		(*i)++;
		return (TOKEN_RPAREN);
	}
	return (TOKEN_WORD);
}

static char	*extract_word(char *input, int *i, t_shell *shell)
{
	int		start;
	int		len;
	char	*word;
	int		j;

	start = *i;
	while (input[*i] && !is_whitespace(input[*i])
		&& !is_special_char(input[*i]))
	{
		if (input[*i] == '\'' || input[*i] == '\"')
		{
			char	quote;

			quote = input[*i];
			(*i)++;
			while (input[*i] && input[*i] != quote)
				(*i)++;
			if (input[*i] == quote)
				(*i)++;
		}
		else
			(*i)++;
	}
	len = *i - start;
	word = gc_malloc(shell->cmd_arena, len + 1);
	if (!word)
		return (NULL);
	j = 0;
	while (j < len)
	{
		word[j] = input[start + j];
		j++;
	}
	word[j] = '\0';
	return (word);
}

static char	*get_operator_value(t_token_type type)
{
	if (type == TOKEN_PIPE)
		return ("|");
	else if (type == TOKEN_REDIR_IN)
		return ("<");
	else if (type == TOKEN_REDIR_OUT)
		return (">");
	else if (type == TOKEN_REDIR_APPEND)
		return (">>");
	else if (type == TOKEN_HEREDOC)
		return ("<<");
	else if (type == TOKEN_AND)
		return ("&&");
	else if (type == TOKEN_OR)
		return ("||");
	else if (type == TOKEN_LPAREN)
		return ("(");
	else if (type == TOKEN_RPAREN)
		return (")");
	return ("");
}

t_token	*lexer(char *input, t_shell *shell)
{
	t_token			*tokens;
	char			*word;
	int				i;
	t_token_type	op_type;

	if (!input || !shell)
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
		{
			op_type = handle_operator(input, &i);
			add_token(&tokens, create_token(op_type,
					get_operator_value(op_type), shell->cmd_arena));
		}
		else
		{
			word = extract_word(input, &i, shell);
			if (word)
				add_token(&tokens, create_token(TOKEN_WORD, word,
						shell->cmd_arena));
		}
	}
	return (tokens);
}
