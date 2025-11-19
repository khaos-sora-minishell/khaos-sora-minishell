#include "minishell.h"

/*
 * Cleanup functions
 */
void	free_tokens(t_token *tokens)
{
	(void)tokens;
	// TODO: Implement token cleanup (GC handles this)
}

void	free_ast(t_ast_node *ast)
{
	(void)ast;
	// TODO: Implement AST cleanup (GC handles this)
}
