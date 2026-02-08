/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_ast_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/01 21:57:48 by akivam            #+#    #+#             */
/*   Updated: 2026/01/01 22:08:47 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

t_ast_node	*new_ast_node(t_node_type type, t_shell *shell)
{
	t_ast_node	*node;

	node = gc_calloc(shell->cmd_arena, 1, sizeof(t_ast_node));
	if (node)
		node->type = type;
	return (node);
}

t_ast_node	*parse_logic(t_token **current, t_shell *shell)
{
	t_ast_node	*node;

	node = parse_pipe(current, shell);
	return (node);
}

t_ast_node	*parse_pipe(t_token **current, t_shell *shell)
{
	t_ast_node	*node;
	t_ast_node	*right;
	t_ast_node	*pipe_node;

	node = parse_primary(current, shell);
	while (*current && (*current)->type == TOKEN_PIPE)
	{
		*current = (*current)->next;
		right = parse_primary(current, shell);
		pipe_node = new_ast_node(NODE_PIPE, shell);
		pipe_node->left = node;
		pipe_node->right = right;
		node = pipe_node;
	}
	return (node);
}
