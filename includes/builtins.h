#ifndef BUILTINS_H
# define BUILTINS_H

#include "minishell.h"

/* ========== BUILTINS ========== */
int	is_builtin(char *cmd);
int		execute_builtin(char **args, t_shell *shell);
int		builtin_echo(char **args);
int		builtin_cd(char **args, t_shell *shell);
int		builtin_pwd(void);
int		builtin_export(char **args, t_shell *shell);
int		builtin_unset(char **args, t_shell *shell);
int		builtin_env(t_shell *shell);
int		builtin_exit(char **args, t_shell *shell);

#endif