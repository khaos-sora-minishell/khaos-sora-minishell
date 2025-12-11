/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 00:38:36 by harici            #+#    #+#             */
/*   Updated: 2025/12/12 00:38:41 by harici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"

/* ========== PARSER INTERNAL FUNCTIONS ========== */

/* AST building functions */
t_ast_node	*build_ast(t_token *tokens, t_shell *shell);
t_ast_node	*create_ast_node(t_node_type type, void *arena);

/* Command parsing */
t_cmd		*parse_simple_command(t_token **tokens, t_shell *shell);

#endif
