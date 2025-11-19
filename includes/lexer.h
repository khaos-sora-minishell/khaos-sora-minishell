#ifndef LEXER_H
# define LEXER_H

# include "./minishell.h"

/* ========== LEXER ========== */
t_token	*lexer(char *input, void *arena);
void	free_tokens(t_token *tokens);
bool	is_metachar(char c);
bool	is_whitespace(char c);

#endif