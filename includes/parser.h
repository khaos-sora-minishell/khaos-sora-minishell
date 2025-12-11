/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 00:02:09 by harici            #+#    #+#             */
/*   Updated: 2025/12/12 00:02:12 by harici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "./minishell.h"

/* ========== PARSER ========== */
t_ast_node	*parser(t_token *tokens, t_shell *shell);
t_cmd		*parse_simple_cmd(t_token **tokens, void *arena);
t_ast_node	*build_ast(t_token *tokens, void *arena);
void		free_ast(t_ast_node *root);

#endif
