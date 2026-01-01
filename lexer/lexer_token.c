/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 04:02:01 by harici            #+#    #+#             */
/*   Updated: 2025/12/22 04:02:02 by harici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"

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

#ifdef BONUS

char	*get_operator_value(t_token_type type)
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

#else

char	*get_operator_value(t_token_type type)
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
	return ("");
}

#endif