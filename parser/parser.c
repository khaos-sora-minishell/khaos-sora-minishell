/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 22:29:47 by akivam            #+#    #+#             */
/*   Updated: 2025/12/20 21:52:03 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

static int	check_syntax(t_token *tokens)
{
	t_token	*current;

	// liste boşsa
	if (!tokens)
		return (0);
	// ilk token pipe olamaz (| ls)
	if (tokens->type == TOKEN_PIPE)
		return (syntax_error("|"), 1);
	// döngüye başla
	current = tokens;
	while (current)
	{
		// şuanki token pipe is
		if (current->type == TOKEN_PIPE)
		{
			if (current->next == NULL)
				return (syntax_error("|"), 1);
			if (current->next->type == TOKEN_PIPE)
				return (syntax_error("|"), 1);
		}
		else if (current->type >= TOKEN_REDIR_IN
			&& current->type <= TOKEN_HEREDOC)
		{
			if (current->next == NULL)
				return (syntax_error("newline"), 1);
			if (current->next->type != TOKEN_WORD)
				return (syntax_error(current->next->value), 1);
		}
		current = current->next;
	}
	return (0);
}

/*
 * Parser - Token listesinden AST oluşturur
 * Input: t_token linked list
 * Output: t_ast_node (AST root)
 */
t_ast_node	*parser(t_token *tokens, t_shell *shell)
{
	if (check_syntax(tokens))
	{
		shell->exit_status = 258;
		return (NULL);
	}
	return (build_ast(tokens, shell));
}
