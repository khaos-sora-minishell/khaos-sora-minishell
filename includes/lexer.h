/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 00:00:57 by harici            #+#    #+#             */
/*   Updated: 2025/12/12 00:00:58 by harici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "./minishell.h"

/* ========== LEXER ========== */
t_token	*lexer(char *input, void *arena);
void	free_tokens(t_token *tokens);
int		is_metachar(char c);

#endif
