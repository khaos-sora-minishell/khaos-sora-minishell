#include "minishell.h"

/*
 * Basit komut parse - Token'lardan t_cmd oluşturur
 * Args ve redirectionları ayıklar
 */
t_cmd	*parse_simple_command(t_token **tokens, t_shell *shell)
{
	t_cmd	*cmd;

	(void)tokens;
	(void)shell;
	cmd = NULL;
	// TODO: Implement simple command parsing
	return (cmd);
}
