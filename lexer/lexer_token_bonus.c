/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_token_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 20:30:07 by harici            #+#    #+#             */
/*   Updated: 2026/01/13 20:30:07 by harici           ###   ########.fr       */
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
