#ifndef PARSER_H
# define PARSER_H

# include "./minishell.h"

/* ========== PARSER ========== */
t_ast_node	*parser(t_token *tokens, t_shell *shell);
t_cmd		*parse_simple_cmd(t_token **tokens, void *arena);
t_ast_node	*build_ast(t_token *tokens, void *arena);
void		free_ast(t_ast_node *root);

#endif