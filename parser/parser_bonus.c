/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 22:29:47 by akivam            #+#    #+#             */
/*   Updated: 2026/01/18 01:48:12 by akivam           ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

static int	validate_token_syntax(t_token *current)
{
	if (current->type == TOKEN_PIPE)
	{
		if (current->next == NULL)
			return (syntax_error("|"), 1);
		if (current->next->type == TOKEN_PIPE)
			return (syntax_error("|"), 1);
	}
	else if (current->type == TOKEN_OR || current->type == TOKEN_AND)
	{
		if (current->next == NULL)
			return (syntax_error("newline"), 1);
	}
	else if (current->type >= TOKEN_REDIR_IN && current->type <= TOKEN_HEREDOC)
	{
		if (current->next == NULL)
			return (syntax_error("newline"), 1);
		if (current->next->type != TOKEN_WORD)
			return (syntax_error(current->next->value), 1);
	}
	return (0);
}

static int	check_syntax(t_token *tokens)
{
	t_token	*current;

	if (!tokens)
		return (0);
	if (tokens->type == TOKEN_PIPE || tokens->type == TOKEN_OR
		|| tokens->type == TOKEN_AND)
		return (syntax_error(tokens->value), 1);
	current = tokens;
	while (current)
	{
		if (validate_token_syntax(current))
			return (1);
		current = current->next;
	}
	return (0);
}

t_ast_node	*parser(t_token *tokens, t_shell *shell)
{
	if (check_syntax(tokens))
	{
		shell->exit_status = 258;
		return (NULL);
	}
	return (build_ast(tokens, shell));
}
