/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 16:36:35 by akivam            #+#    #+#             */
/*   Updated: 2025/12/22 16:14:32 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

static t_ast_node	*parse_pipe(t_token **current, t_shell *shell);
static t_ast_node	*parse_primary(t_token **current, t_shell *shell);

static t_ast_node	*new_ast_node(t_node_type type, t_shell *shell)
{
	t_ast_node	*node;

	node = gc_calloc(shell->cmd_arena, 1, sizeof(t_ast_node));
	if (node)
		node->type = type;
	return (node);
}

static t_ast_node	*parse_logic(t_token **current, t_shell *shell)
{
	t_ast_node	*node;
	t_ast_node	*right;
	t_ast_node	*parent;
	t_node_type	type;

	node = parse_pipe(current, shell);
	while (*current && ((*current)->type == TOKEN_AND
			|| (*current)->type == TOKEN_OR))
	{
		type = NODE_OR;
		if ((*current)->type == TOKEN_AND)
			type = NODE_AND;
		*current = (*current)->next;
		right = parse_pipe(current, shell);
		parent = new_ast_node(type, shell);
		parent->left = node;
		parent->right = right;
		node = parent;
	}
	return (node);
}

static t_ast_node	*parse_pipe(t_token **current, t_shell *shell)
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

static t_ast_node	*parse_primary(t_token **current, t_shell *shell)
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
