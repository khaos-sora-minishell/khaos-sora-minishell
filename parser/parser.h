/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 00:38:36 by harici            #+#    #+#             */
/*   Updated: 2025/12/22 16:45:25 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"

t_ast_node	*build_ast(t_token *tokens, t_shell *shell);
t_ast_node	*create_ast_node(t_node_type type, void *arena);

t_cmd		*parse_simple_command(t_token **tokens, t_shell *shell);

int			is_redirection_token(t_token_type type);
int			is_operator_token(t_token_type type);
t_cmd		*create_cmd(void *arena);
void		add_arg_to_cmd(t_cmd *cmd, char *arg, void *arena);
t_redir		*create_redir(t_token_type type, char *file, void *arena);
void		add_redir(t_redir **list, t_redir *new_redir);

#endif
