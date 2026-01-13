/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 20:31:08 by harici            #+#    #+#             */
/*   Updated: 2026/01/13 20:31:08 by harici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "minishell.h"

char			*get_operator_value(t_token_type type);

t_token_type	handle_operator(char *input, int *i);
t_token_type	handle_double_op(char c, char next, int *i);
t_token_type	handle_single_op(char c, int *i);

char			*extract_word(char *input, int *i, t_shell *shell);
void			skip_quoted(char *input, int *i);
void			find_word_end(char *input, int *i);

char			check_unclosed_quotes_char(char *input);
t_token			*expand_alias_if_needed(t_token *tokens, t_shell *shell);

#endif
