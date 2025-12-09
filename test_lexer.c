#include "minishell.h"
#include "printf.h"

/*
 * print_tokens - Debug helper to print token list
 * @tokens: Head of token linked list
 *
 * Prints each token's type and value for debugging
 */
void	print_tokens(t_token *tokens)
{
	const char	*type_names[] = {
		"WORD", "PIPE", "REDIR_IN", "REDIR_OUT",
		"REDIR_APPEND", "HEREDOC", "AND", "OR",
		"LPAREN", "RPAREN"
	};
	t_token		*current;
	int			count;

	current = tokens;
	count = 0;
	ft_printf("\n=== TOKENS ===\n");
	while (current)
	{
		ft_printf("[%d] Type: %s | Value: \"%s\"\n",
			count++, type_names[current->type], current->value);
		current = current->next;
	}
	ft_printf("Total tokens: %d\n\n", count);
}

/*
 * test_lexer - Test function for lexer
 *
 * Tests various input strings and displays tokenization results
 */
void	test_lexer(void)
{
	char		*tests[] = {
		"ls -la",
		"cat file.txt",
		"echo hello world",
		"grep test file1 file2",
		"ls    -la     /tmp",
		"cat filex.txt | cat -e | wc -l",
		NULL
	};
	t_shell		shell;
	int			i;

	// Initialize shell with GC arena
	shell.cmd_arena = gc_create();
	if (!shell.cmd_arena)
	{
		ft_printf("Error: Failed to create GC arena\n");
		return ;
	}

	ft_printf("\n========================================\n");
	ft_printf("    LEXER BASIC TOKENIZATION TEST\n");
	ft_printf("========================================\n");

	i = 0;
	while (tests[i])
	{
		ft_printf("\nTest %d: \"%s\"\n", i + 1, tests[i]);
		ft_printf("----------------------------------------\n");

		t_token *tokens = lexer(tests[i], &shell);
		if (tokens)
			print_tokens(tokens);
		else
			ft_printf("Error: Lexer returned NULL\n");

		// Clean up for next test
		gc_destroy(shell.cmd_arena);
		shell.cmd_arena = gc_create();
		i++;
	}

	// Cleanup
	gc_destroy(shell.cmd_arena);
	ft_printf("\n========================================\n");
	ft_printf("           TESTS COMPLETED\n");
	ft_printf("========================================\n\n");
}

/*
 * main - Entry point for lexer testing
 * @ac: Argument count (unused)
 * @av: Argument vector (unused)
 * @envp: Environment variables (unused)
 *
 * Return: 0 on success
 */
int	main(int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;
	(void)envp;
	test_lexer();
	return (0);
}
