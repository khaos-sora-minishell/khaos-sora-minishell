#include "minishell.h"

int	builtin_true(char **args, t_shell *shell)
{
	(void)args;
	(void)shell;
	return (0);
}

int	builtin_false(char **args, t_shell *shell)
{
	(void)args;
	(void)shell;
	return (1);
}
