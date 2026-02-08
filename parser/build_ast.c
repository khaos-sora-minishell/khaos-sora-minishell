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

t_ast_node	*parse_primary(t_token **current, t_shell *shell)
{
	t_ast_node	*node;

	if (!*current)
		return (NULL);
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
