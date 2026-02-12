/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_operator_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 20:29:39 by harici            #+#    #+#             */
/*   Updated: 2026/01/13 20:29:39 by harici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "lexer.h"

t_token_type	handle_double_op(char c, char next, int *i)
{
	if (c == '|' && next == '|')
	{
		(*i) += 2;
		return (TOKEN_OR);
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
	else if (c == '>' && next == '>')
	{
		(*i) += 2;
		return (TOKEN_REDIR_APPEND);
	}
	return (TOKEN_WORD);
}

t_token_type	handle_single_op(char c, int *i)
{
	if (c == '|')
		return ((*i)++, TOKEN_PIPE);
	else if (c == '<')
		return ((*i)++, TOKEN_REDIR_IN);
	else if (c == '>')
		return ((*i)++, TOKEN_REDIR_OUT);
	else if (c == '(')
		return ((*i)++, TOKEN_LPAREN);
	else if (c == ')')
		return ((*i)++, TOKEN_RPAREN);
	else if (c == '&')
	{
		(*i)++;
		return (TOKEN_WORD);
	}
	return (TOKEN_WORD);
}

t_token_type	handle_operator(char *input, int *i)
{
	t_token_type	type;

	type = handle_double_op(input[*i], input[*i + 1], i);
	if (type != TOKEN_WORD)
		return (type);
	return (handle_single_op(input[*i], i));
}
