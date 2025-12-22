/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_operator.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 04:01:54 by harici            #+#    #+#             */
/*   Updated: 2025/12/22 04:01:56 by harici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
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
