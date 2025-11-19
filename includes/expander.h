#ifndef EXPANDER_H
# define EXPANDER_H

# include "minishell.h"
/* ========== EXPANDER ========== */
void	expand_ast(t_ast_node *root, t_shell *shell);
char	*expand_variables(char *str, t_shell *shell);
char	*expand_wildcard(char *pattern, void *arena);
char	*get_exit_status_str(int status, void *arena);

#endif