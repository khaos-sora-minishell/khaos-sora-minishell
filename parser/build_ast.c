/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 16:36:35 by akivam            #+#    #+#             */
/*   Updated: 2025/12/21 22:13:25 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

static t_ast_node	*new_ast_node(t_node_type type, t_shell *shell)
{
	t_ast_node	*node;

	node = gc_calloc(shell->cmd_arena, 1, sizeof(t_ast_node));
	if (node)
		node->type = type;
	return (node);
}

static t_ast_node	*create_cmd_node(t_token **tokens, t_shell *shell)
{
	t_ast_node	*node;
	t_cmd		*cmd;

	cmd = parse_simple_command(tokens, shell);
	if (!cmd)
		return (NULL);
	node = new_ast_node(NODE_CMD, shell);
	if (!node)
		return (NULL);
	node->cmd = cmd;
	return (node);
}

t_ast_node	*build_ast(t_token *tokens, t_shell *shell)
{
	t_ast_node	*root;
	t_ast_node	*right;
	t_ast_node	*pipe_node;
	t_token		*current;

	current = tokens;
	root = create_cmd_node(&current, shell);
	if (!root)
		return (NULL);
	while (current && current->type == TOKEN_PIPE)
	{
		current = current->next;
		right = create_cmd_node(&current, shell);
		if (!right)
			return (NULL);
		pipe_node = new_ast_node(NODE_PIPE, shell);
		if (!pipe_node)
			return (NULL);
		pipe_node->left = root;
		pipe_node->right = right;
		root = pipe_node;
	}
	return (root);
}
