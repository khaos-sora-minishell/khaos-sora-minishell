/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 16:36:35 by akivam            #+#    #+#             */
/*   Updated: 2025/12/20 16:47:00 by akivam           ###   ########.fr       */
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
	node->cmd = cmd;
	return (node);
}

/*
 * AST oluşturma - t_cmd'leri kullanarak ağaç kurar
 * Pipe, AND, OR operatörlerini ekler
 */
t_ast_node	*build_ast(t_token *tokens, t_shell *shell)
{
	t_ast_node *root;      // en üst düğüm
	t_ast_node *right;     // sağ taraf
	t_ast_node *pipe_node; // pipe bağlantısı
	t_token *current;      // tokende gezen parça
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
		pipe_node->left = root;
		pipe_node->right = right;
		root = pipe_node;
	}
	return (root);
}
