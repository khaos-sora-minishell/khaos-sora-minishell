#include "minishell.h"
#include <stdio.h>

/*
 * Print error message
 */
void	print_error(char *cmd, char *msg)
{
	fprintf(stderr, "minishell: %s: %s\n", cmd, msg);
}

/*
 * Print syntax error
 */
void	syntax_error(char *token)
{
	fprintf(stderr, "minishell: syntax error near unexpected token `%s'\n", token);
}

/*
 * Print command not found error
 */
int	command_not_found(char *cmd)
{
	fprintf(stderr, "minishell: %s: command not found\n", cmd);
	return (127);
}

/*
 * Print error and exit
 */
void	error_exit(char *msg, t_shell *shell)
{
	(void)shell;
	fprintf(stderr, "minishell: %s\n", msg);
	exit(1);
}
