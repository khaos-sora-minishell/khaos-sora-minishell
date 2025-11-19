#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "minishell.h"

/* ========== EXECUTOR ========== */
int		executor(t_ast_node *root, t_shell *shell);
void	execute_cmd(t_cmd *cmd, t_shell *shell);
void	execute_pipe(t_ast_node *node, t_shell *shell);
void	execute_and(t_ast_node *node, t_shell *shell);
void	execute_or(t_ast_node *node, t_shell *shell);
void	execute_subshell(t_ast_node *node, t_shell *shell);

#endif