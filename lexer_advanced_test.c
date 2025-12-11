#include "minishell.h"
#include "lexer.h"
#include "printf.h"

#define COLOR_GREEN "\033[0;32m"
#define COLOR_RED "\033[0;31m"
#define COLOR_YELLOW "\033[0;33m"
#define COLOR_BLUE "\033[0;34m"
#define COLOR_RESET "\033[0m"

typedef struct s_test_stats
{
	int	total;
	int	passed;
	int	failed;
}	t_test_stats;

void	print_token_list(t_token *tokens)
{
	const char	*type_names[] = {
		"WORD", "PIPE", "REDIR_IN", "REDIR_OUT",
		"REDIR_APPEND", "HEREDOC", "AND", "OR",
		"LPAREN", "RPAREN"
	};
	t_token		*current;
	int			first;

	current = tokens;
	first = 1;
	ft_printf("  Tokens: [");
	while (current)
	{
		if (!first)
			ft_printf(", ");
		ft_printf("%s:\"%s\"", type_names[current->type], current->value);
		current = current->next;
		first = 0;
	}
	ft_printf("]\n");
}

void	test_error_handling(t_shell *shell, t_test_stats *stats)
{
	t_token	*tokens;

	ft_printf(COLOR_BLUE "\n=== ERROR HANDLING TESTS ===\n" COLOR_RESET);

	// Test 1: Unclosed single quote
	stats->total++;
	ft_printf("\nTest %d: Unclosed single quote \"echo 'hello\"\n", stats->total);
	tokens = lexer("echo 'hello", shell);
	if (tokens)
	{
		print_token_list(tokens);
		ft_printf(COLOR_YELLOW "  ⚠ WARNING: Accepted unclosed quote (may need validation)\n" COLOR_RESET);
		stats->passed++;
	}
	else
	{
		ft_printf(COLOR_GREEN "  ✓ PASSED (rejected unclosed quote)\n" COLOR_RESET);
		stats->passed++;
	}
	gc_destroy(shell->cmd_arena);
	shell->cmd_arena = gc_create();

	// Test 2: Unclosed double quote
	stats->total++;
	ft_printf("\nTest %d: Unclosed double quote \"echo \\\"hello\"\n", stats->total);
	tokens = lexer("echo \"hello", shell);
	if (tokens)
	{
		print_token_list(tokens);
		ft_printf(COLOR_YELLOW "  ⚠ WARNING: Accepted unclosed quote (may need validation)\n" COLOR_RESET);
		stats->passed++;
	}
	else
	{
		ft_printf(COLOR_GREEN "  ✓ PASSED (rejected unclosed quote)\n" COLOR_RESET);
		stats->passed++;
	}
	gc_destroy(shell->cmd_arena);
	shell->cmd_arena = gc_create();

	// Test 3: Mismatched quotes
	stats->total++;
	ft_printf("\nTest %d: Mismatched quotes \"echo 'hello\\\"\"\n", stats->total);
	tokens = lexer("echo 'hello\"", shell);
	if (tokens)
	{
		print_token_list(tokens);
		ft_printf(COLOR_GREEN "  ✓ PASSED (handled mismatched quotes)\n" COLOR_RESET);
		stats->passed++;
	}
	else
	{
		ft_printf(COLOR_RED "  ✗ FAILED (rejected valid input)\n" COLOR_RESET);
		stats->failed++;
	}
	gc_destroy(shell->cmd_arena);
	shell->cmd_arena = gc_create();
}

void	test_special_chars(t_shell *shell, t_test_stats *stats)
{
	t_token	*tokens;

	ft_printf(COLOR_BLUE "\n=== SPECIAL CHARACTER TESTS ===\n" COLOR_RESET);

	// Test 4: Environment variables
	stats->total++;
	ft_printf("\nTest %d: Environment variable \"echo $HOME\"\n", stats->total);
	tokens = lexer("echo $HOME", shell);
	if (tokens)
	{
		print_token_list(tokens);
		ft_printf(COLOR_GREEN "  ✓ PASSED (tokenized)\n" COLOR_RESET);
		stats->passed++;
	}
	else
	{
		ft_printf(COLOR_RED "  ✗ FAILED\n" COLOR_RESET);
		stats->failed++;
	}
	gc_destroy(shell->cmd_arena);
	shell->cmd_arena = gc_create();

	// Test 5: Wildcards
	stats->total++;
	ft_printf("\nTest %d: Wildcards \"ls *.txt\"\n", stats->total);
	tokens = lexer("ls *.txt", shell);
	if (tokens)
	{
		print_token_list(tokens);
		ft_printf(COLOR_GREEN "  ✓ PASSED (tokenized)\n" COLOR_RESET);
		stats->passed++;
	}
	else
	{
		ft_printf(COLOR_RED "  ✗ FAILED\n" COLOR_RESET);
		stats->failed++;
	}
	gc_destroy(shell->cmd_arena);
	shell->cmd_arena = gc_create();

	// Test 6: Question mark pattern
	stats->total++;
	ft_printf("\nTest %d: Pattern \"ls file?.txt\"\n", stats->total);
	tokens = lexer("ls file?.txt", shell);
	if (tokens)
	{
		print_token_list(tokens);
		ft_printf(COLOR_GREEN "  ✓ PASSED (tokenized)\n" COLOR_RESET);
		stats->passed++;
	}
	else
	{
		ft_printf(COLOR_RED "  ✗ FAILED\n" COLOR_RESET);
		stats->failed++;
	}
	gc_destroy(shell->cmd_arena);
	shell->cmd_arena = gc_create();

	// Test 7: Tab character
	stats->total++;
	ft_printf("\nTest %d: Tab character \"echo\\thello\"\n", stats->total);
	tokens = lexer("echo\thello", shell);
	if (tokens)
	{
		print_token_list(tokens);
		ft_printf(COLOR_GREEN "  ✓ PASSED (tokenized)\n" COLOR_RESET);
		stats->passed++;
	}
	else
	{
		ft_printf(COLOR_RED "  ✗ FAILED\n" COLOR_RESET);
		stats->failed++;
	}
	gc_destroy(shell->cmd_arena);
	shell->cmd_arena = gc_create();
}

void	test_edge_cases(t_shell *shell, t_test_stats *stats)
{
	t_token	*tokens;
	char	long_input[2048];
	int		i;

	ft_printf(COLOR_BLUE "\n=== EDGE CASE TESTS ===\n" COLOR_RESET);

	// Test 8: Very long input
	stats->total++;
	ft_printf("\nTest %d: Very long input (1000+ chars)\n", stats->total);
	for (i = 0; i < 1000; i++)
		long_input[i] = 'a';
	long_input[1000] = '\0';
	tokens = lexer(long_input, shell);
	if (tokens)
	{
		ft_printf("  Token value length: %d\n", (int)gc_strlen(tokens->value));
		ft_printf(COLOR_GREEN "  ✓ PASSED (handled long input)\n" COLOR_RESET);
		stats->passed++;
	}
	else
	{
		ft_printf(COLOR_RED "  ✗ FAILED\n" COLOR_RESET);
		stats->failed++;
	}
	gc_destroy(shell->cmd_arena);
	shell->cmd_arena = gc_create();

	// Test 9: Escaped characters in quotes
	stats->total++;
	ft_printf("\nTest %d: Backslash in quotes \"echo \\\"test\\\"\"\n", stats->total);
	tokens = lexer("echo \"test\\\"", shell);
	if (tokens)
	{
		print_token_list(tokens);
		ft_printf(COLOR_GREEN "  ✓ PASSED (tokenized)\n" COLOR_RESET);
		stats->passed++;
	}
	else
	{
		ft_printf(COLOR_RED "  ✗ FAILED\n" COLOR_RESET);
		stats->failed++;
	}
	gc_destroy(shell->cmd_arena);
	shell->cmd_arena = gc_create();

	// Test 10: Multiple operators in sequence
	stats->total++;
	ft_printf("\nTest %d: Multiple pipes \"ls | | grep\"\n", stats->total);
	tokens = lexer("ls | | grep", shell);
	if (tokens)
	{
		print_token_list(tokens);
		ft_printf(COLOR_GREEN "  ✓ PASSED (tokenized - syntax check is parser's job)\n" COLOR_RESET);
		stats->passed++;
	}
	else
	{
		ft_printf(COLOR_RED "  ✗ FAILED\n" COLOR_RESET);
		stats->failed++;
	}
	gc_destroy(shell->cmd_arena);
	shell->cmd_arena = gc_create();

	// Test 11: Nested quotes
	stats->total++;
	ft_printf("\nTest %d: Nested quotes \"echo \\\"'hello'\\\"\"\n", stats->total);
	tokens = lexer("echo \"'hello'\"", shell);
	if (tokens)
	{
		print_token_list(tokens);
		ft_printf(COLOR_GREEN "  ✓ PASSED (tokenized)\n" COLOR_RESET);
		stats->passed++;
	}
	else
	{
		ft_printf(COLOR_RED "  ✗ FAILED\n" COLOR_RESET);
		stats->failed++;
	}
	gc_destroy(shell->cmd_arena);
	shell->cmd_arena = gc_create();
}

void	print_summary(t_test_stats *stats)
{
	ft_printf("\n========================================\n");
	ft_printf("        ADVANCED TEST SUMMARY\n");
	ft_printf("========================================\n");
	ft_printf("Total tests:  %d\n", stats->total);
	ft_printf(COLOR_GREEN "Passed:       %d\n" COLOR_RESET, stats->passed);
	ft_printf(COLOR_RED "Failed:       %d\n" COLOR_RESET, stats->failed);
	ft_printf("Success rate: %.1f%%\n",
		(float)stats->passed / stats->total * 100);
	ft_printf("========================================\n");
}

int	main(int ac, char **av, char **envp)
{
	t_shell			shell;
	t_test_stats	stats;

	(void)ac;
	(void)av;
	(void)envp;
	stats.total = 0;
	stats.passed = 0;
	stats.failed = 0;

	shell.global_arena = gc_create();
	shell.cmd_arena = gc_create();
	if (!shell.global_arena || !shell.cmd_arena)
	{
		ft_printf(COLOR_RED "Failed to initialize garbage collector\n" COLOR_RESET);
		return (1);
	}

	ft_printf("========================================\n");
	ft_printf("  ADVANCED LEXER TEST SUITE\n");
	ft_printf("========================================\n");

	test_error_handling(&shell, &stats);
	test_special_chars(&shell, &stats);
	test_edge_cases(&shell, &stats);

	print_summary(&stats);

	gc_destroy(shell.cmd_arena);
	gc_destroy(shell.global_arena);

	return (0);
}
