#include "minishell.h"

/*
 * Parser - Token listesinden AST oluşturur
 * Input: t_token linked list
 * Output: t_ast_node (AST root)
 */
t_ast_node	*parser(t_token *tokens, t_shell *shell)
{
	t_ast_node	*ast;

	(void)tokens;
	(void)shell;
	ast = NULL;
	// TODO: Implement parser
	return (ast);
}
