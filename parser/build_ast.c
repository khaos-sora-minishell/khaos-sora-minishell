#include "minishell.h"

/*
 * AST oluşturma - t_cmd'leri kullanarak ağaç kurar
 * Pipe, AND, OR operatörlerini ekler
 */
t_ast_node	*build_ast(t_token *tokens, t_shell *shell)
{
	t_ast_node	*root;

	(void)tokens;
	(void)shell;
	root = NULL;
	// TODO: Implement AST building
	return (root);
}
