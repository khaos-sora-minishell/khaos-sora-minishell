/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_ast_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 16:36:35 by akivam            #+#    #+#             */
/*   Updated: 2026/01/18 01:48:12 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "parser.h"

t_ast_node	*parse_primary(t_token **current, t_shell *shell)
{
	t_ast_node	*node;

	if (!*current)
		return (NULL);
	if ((*current)->type == TOKEN_LPAREN)
	{
		*current = (*current)->next;
		node = new_ast_node(NODE_SUBSHELL, shell);
		node->subshell_node = parse_logic(current, shell);
		if (*current && (*current)->type == TOKEN_RPAREN)
			*current = (*current)->next;
		return (node);
	}
	node = new_ast_node(NODE_CMD, shell);
	node->cmd = parse_simple_command(current, shell);
	return (node);
}

t_ast_node	*build_ast(t_token *tokens, t_shell *shell)
{
	t_token	*current;

	current = tokens;
	if (!tokens)
		return (NULL);
	return (parse_logic(&current, shell));
}
