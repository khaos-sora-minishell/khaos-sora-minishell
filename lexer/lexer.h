/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 00:38:24 by harici            #+#    #+#             */
/*   Updated: 2025/12/22 04:31:26 by harici           ###   ########.fr       */
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

#endif
