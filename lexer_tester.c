#include "minishell.h"
#include "printf.h"

/*
 * Color codes for output
 */
#define COLOR_GREEN "\033[0;32m"
#define COLOR_RED "\033[0;31m"
#define COLOR_YELLOW "\033[0;33m"
#define COLOR_BLUE "\033[0;34m"
#define COLOR_RESET "\033[0m"

/*
 * Test result counters
 */
typedef struct s_test_stats
{
	int	total;
	int	passed;
	int	failed;
}	t_test_stats;

/*
 * print_token_list - Prints tokens in compact format
 */
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

/*
 * verify_token - Verifies a single token
 */
int	verify_token(t_token *token, t_token_type expected_type,
		const char *expected_value)
{
	if (!token)
	{
		ft_printf(COLOR_RED "    ✗ Expected token not found\n" COLOR_RESET);
		return (0);
	}
	if (token->type != expected_type)
	{
		ft_printf(COLOR_RED "    ✗ Type mismatch: expected %d, got %d\n"
			COLOR_RESET, expected_type, token->type);
		return (0);
	}
	if (gc_strcmp(token->value, expected_value) != 0)
	{
		ft_printf(COLOR_RED "    ✗ Value mismatch: expected \"%s\", got \"%s\"\n"
			COLOR_RESET, expected_value, token->value);
		return (0);
	}
	return (1);
}

/*
 * run_basic_test - Tests basic tokenization
 */
void	run_basic_test(t_shell *shell, t_test_stats *stats)
{
	t_token	*tokens;
	int		success;

	ft_printf(COLOR_BLUE "\n=== BASIC TOKENIZATION TESTS ===\n" COLOR_RESET);

	// Test 1: Simple command
	stats->total++;
	ft_printf("\nTest %d: Simple command \"ls -la\"\n", stats->total);
	tokens = lexer("ls -la", shell);
	print_token_list(tokens);
	success = verify_token(tokens, TOKEN_WORD, "ls");
	success = success && verify_token(tokens->next, TOKEN_WORD, "-la");
	success = success && (tokens->next->next == NULL);
	if (success)
	{
		ft_printf(COLOR_GREEN "  ✓ PASSED\n" COLOR_RESET);
		stats->passed++;
	}
	else
	{
		ft_printf(COLOR_RED "  ✗ FAILED\n" COLOR_RESET);
		stats->failed++;
	}
	gc_destroy(shell->cmd_arena);
	shell->cmd_arena = gc_create();

	// Test 2: Multiple arguments
	stats->total++;
	ft_printf("\nTest %d: Multiple arguments \"echo hello world\"\n", stats->total);
	tokens = lexer("echo hello world", shell);
	print_token_list(tokens);
	success = verify_token(tokens, TOKEN_WORD, "echo");
	success = success && verify_token(tokens->next, TOKEN_WORD, "hello");
	success = success && verify_token(tokens->next->next, TOKEN_WORD, "world");
	if (success)
	{
		ft_printf(COLOR_GREEN "  ✓ PASSED\n" COLOR_RESET);
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

/*
 * run_operator_test - Tests operator tokenization
 */
void	run_operator_test(t_shell *shell, t_test_stats *stats)
{
	t_token	*tokens;
	int		success;

	ft_printf(COLOR_BLUE "\n=== OPERATOR TOKENIZATION TESTS ===\n" COLOR_RESET);

	// Test: Pipe
	stats->total++;
	ft_printf("\nTest %d: Pipe \"cat file.txt | grep test\"\n", stats->total);
	tokens = lexer("cat file.txt | grep test", shell);
	print_token_list(tokens);
	success = verify_token(tokens, TOKEN_WORD, "cat");
	success = success && verify_token(tokens->next, TOKEN_WORD, "file.txt");
	success = success && verify_token(tokens->next->next, TOKEN_PIPE, "|");
	success = success && verify_token(tokens->next->next->next, TOKEN_WORD, "grep");
	if (success)
	{
		ft_printf(COLOR_GREEN "  ✓ PASSED\n" COLOR_RESET);
		stats->passed++;
	}
	else
	{
		ft_printf(COLOR_RED "  ✗ FAILED\n" COLOR_RESET);
		stats->failed++;
	}
	gc_destroy(shell->cmd_arena);
	shell->cmd_arena = gc_create();

	// Test: Redirections
	stats->total++;
	ft_printf("\nTest %d: Redirections \"cat < in.txt > out.txt\"\n",
		stats->total);
	tokens = lexer("cat < in.txt > out.txt", shell);
	print_token_list(tokens);
	success = verify_token(tokens, TOKEN_WORD, "cat");
	success = success && verify_token(tokens->next, TOKEN_REDIR_IN, "<");
	success = success && verify_token(tokens->next->next, TOKEN_WORD, "in.txt");
	success = success && verify_token(tokens->next->next->next,
			TOKEN_REDIR_OUT, ">");
	if (success)
	{
		ft_printf(COLOR_GREEN "  ✓ PASSED\n" COLOR_RESET);
		stats->passed++;
	}
	else
	{
		ft_printf(COLOR_RED "  ✗ FAILED\n" COLOR_RESET);
		stats->failed++;
	}
	gc_destroy(shell->cmd_arena);
	shell->cmd_arena = gc_create();

	// Test: Append and Heredoc
	stats->total++;
	ft_printf("\nTest %d: Append & Heredoc \"echo test >> file << EOF\"\n",
		stats->total);
	tokens = lexer("echo test >> file << EOF", shell);
	print_token_list(tokens);
	success = verify_token(tokens, TOKEN_WORD, "echo");
	success = success && verify_token(tokens->next, TOKEN_WORD, "test");
	success = success && verify_token(tokens->next->next,
			TOKEN_REDIR_APPEND, ">>");
	success = success && verify_token(tokens->next->next->next,
			TOKEN_WORD, "file");
	success = success && verify_token(tokens->next->next->next->next,
			TOKEN_HEREDOC, "<<");
	if (success)
	{
		ft_printf(COLOR_GREEN "  ✓ PASSED\n" COLOR_RESET);
		stats->passed++;
	}
	else
	{
		ft_printf(COLOR_RED "  ✗ FAILED\n" COLOR_RESET);
		stats->failed++;
	}
	gc_destroy(shell->cmd_arena);
	shell->cmd_arena = gc_create();

	// Test: Logical operators
	stats->total++;
	ft_printf("\nTest %d: Logical operators \"ls && echo ok || echo fail\"\n",
		stats->total);
	tokens = lexer("ls && echo ok || echo fail", shell);
	print_token_list(tokens);
	success = verify_token(tokens, TOKEN_WORD, "ls");
	success = success && verify_token(tokens->next, TOKEN_AND, "&&");
	success = success && verify_token(tokens->next->next, TOKEN_WORD, "echo");
	success = success && verify_token(tokens->next->next->next,
			TOKEN_WORD, "ok");
	success = success && verify_token(tokens->next->next->next->next,
			TOKEN_OR, "||");
	if (success)
	{
		ft_printf(COLOR_GREEN "  ✓ PASSED\n" COLOR_RESET);
		stats->passed++;
	}
	else
	{
		ft_printf(COLOR_RED "  ✗ FAILED\n" COLOR_RESET);
		stats->failed++;
	}
	gc_destroy(shell->cmd_arena);
	shell->cmd_arena = gc_create();

	// Test: Parentheses
	stats->total++;
	ft_printf("\nTest %d: Parentheses \"(ls | grep txt) && echo done\"\n",
		stats->total);
	tokens = lexer("(ls | grep txt) && echo done", shell);
	print_token_list(tokens);
	success = verify_token(tokens, TOKEN_LPAREN, "(");
	success = success && verify_token(tokens->next, TOKEN_WORD, "ls");
	success = success && verify_token(tokens->next->next, TOKEN_PIPE, "|");
	success = success && verify_token(tokens->next->next->next,
			TOKEN_WORD, "grep");
	success = success && verify_token(tokens->next->next->next->next,
			TOKEN_WORD, "txt");
	success = success && verify_token(tokens->next->next->next->next->next,
			TOKEN_RPAREN, ")");
	if (success)
	{
		ft_printf(COLOR_GREEN "  ✓ PASSED\n" COLOR_RESET);
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

/*
 * run_quote_test - Tests quote handling
 */
void	run_quote_test(t_shell *shell, t_test_stats *stats)
{
	t_token	*tokens;
	int		success;

	ft_printf(COLOR_BLUE "\n=== QUOTE HANDLING TESTS ===\n" COLOR_RESET);

	// Test: Single quotes
	stats->total++;
	ft_printf("\nTest %d: Single quotes \"echo 'hello world'\"\n", stats->total);
	tokens = lexer("echo 'hello world'", shell);
	print_token_list(tokens);
	success = verify_token(tokens, TOKEN_WORD, "echo");
	success = success && verify_token(tokens->next, TOKEN_WORD,
			"'hello world'");
	if (success)
	{
		ft_printf(COLOR_GREEN "  ✓ PASSED\n" COLOR_RESET);
		stats->passed++;
	}
	else
	{
		ft_printf(COLOR_RED "  ✗ FAILED\n" COLOR_RESET);
		stats->failed++;
	}
	gc_destroy(shell->cmd_arena);
	shell->cmd_arena = gc_create();

	// Test: Double quotes
	stats->total++;
	ft_printf("\nTest %d: Double quotes \"echo \\\"hello world\\\"\"\n",
		stats->total);
	tokens = lexer("echo \"hello world\"", shell);
	print_token_list(tokens);
	success = verify_token(tokens, TOKEN_WORD, "echo");
	success = success && verify_token(tokens->next, TOKEN_WORD,
			"\"hello world\"");
	if (success)
	{
		ft_printf(COLOR_GREEN "  ✓ PASSED\n" COLOR_RESET);
		stats->passed++;
	}
	else
	{
		ft_printf(COLOR_RED "  ✗ FAILED\n" COLOR_RESET);
		stats->failed++;
	}
	gc_destroy(shell->cmd_arena);
	shell->cmd_arena = gc_create();

	// Test: Mixed quotes
	stats->total++;
	ft_printf("\nTest %d: Mixed quotes \"echo \\\"it's ok\\\"\"\n", stats->total);
	tokens = lexer("echo \"it's ok\"", shell);
	print_token_list(tokens);
	success = verify_token(tokens, TOKEN_WORD, "echo");
	success = success && verify_token(tokens->next, TOKEN_WORD, "\"it's ok\"");
	if (success)
	{
		ft_printf(COLOR_GREEN "  ✓ PASSED\n" COLOR_RESET);
		stats->passed++;
	}
	else
	{
		ft_printf(COLOR_RED "  ✗ FAILED\n" COLOR_RESET);
		stats->failed++;
	}
	gc_destroy(shell->cmd_arena);
	shell->cmd_arena = gc_create();

	// Test: Empty quotes
	stats->total++;
	ft_printf("\nTest %d: Empty quotes \"echo '' \\\"\\\"\"\n", stats->total);
	tokens = lexer("echo '' \"\"", shell);
	print_token_list(tokens);
	success = verify_token(tokens, TOKEN_WORD, "echo");
	success = success && verify_token(tokens->next, TOKEN_WORD, "''");
	success = success && verify_token(tokens->next->next, TOKEN_WORD, "\"\"");
	if (success)
	{
		ft_printf(COLOR_GREEN "  ✓ PASSED\n" COLOR_RESET);
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

/*
 * run_edge_case_test - Tests edge cases
 */
void	run_edge_case_test(t_shell *shell, t_test_stats *stats)
{
	t_token	*tokens;
	int		success;

	ft_printf(COLOR_BLUE "\n=== EDGE CASE TESTS ===\n" COLOR_RESET);

	// Test: Multiple spaces
	stats->total++;
	ft_printf("\nTest %d: Multiple spaces \"ls    -la     /tmp\"\n",
		stats->total);
	tokens = lexer("ls    -la     /tmp", shell);
	print_token_list(tokens);
	success = verify_token(tokens, TOKEN_WORD, "ls");
	success = success && verify_token(tokens->next, TOKEN_WORD, "-la");
	success = success && verify_token(tokens->next->next, TOKEN_WORD, "/tmp");
	success = success && (tokens->next->next->next == NULL);
	if (success)
	{
		ft_printf(COLOR_GREEN "  ✓ PASSED\n" COLOR_RESET);
		stats->passed++;
	}
	else
	{
		ft_printf(COLOR_RED "  ✗ FAILED\n" COLOR_RESET);
		stats->failed++;
	}
	gc_destroy(shell->cmd_arena);
	shell->cmd_arena = gc_create();

	// Test: Empty input
	stats->total++;
	ft_printf("\nTest %d: Empty input \"\"\n", stats->total);
	tokens = lexer("", shell);
	success = (tokens == NULL);
	if (success)
	{
		ft_printf(COLOR_GREEN "  ✓ PASSED (NULL)\n" COLOR_RESET);
		stats->passed++;
	}
	else
	{
		ft_printf(COLOR_RED "  ✗ FAILED\n" COLOR_RESET);
		stats->failed++;
	}
	gc_destroy(shell->cmd_arena);
	shell->cmd_arena = gc_create();

	// Test: Only spaces
	stats->total++;
	ft_printf("\nTest %d: Only spaces \"     \"\n", stats->total);
	tokens = lexer("     ", shell);
	success = (tokens == NULL);
	if (success)
	{
		ft_printf(COLOR_GREEN "  ✓ PASSED (NULL)\n" COLOR_RESET);
		stats->passed++;
	}
	else
	{
		ft_printf(COLOR_RED "  ✗ FAILED\n" COLOR_RESET);
		stats->failed++;
	}
	gc_destroy(shell->cmd_arena);
	shell->cmd_arena = gc_create();

	// Test: Operators without spaces
	stats->total++;
	ft_printf("\nTest %d: No spaces \"cat<in.txt>out.txt\"\n", stats->total);
	tokens = lexer("cat<in.txt>out.txt", shell);
	print_token_list(tokens);
	success = verify_token(tokens, TOKEN_WORD, "cat");
	success = success && verify_token(tokens->next, TOKEN_REDIR_IN, "<");
	success = success && verify_token(tokens->next->next, TOKEN_WORD, "in.txt");
	success = success && verify_token(tokens->next->next->next,
			TOKEN_REDIR_OUT, ">");
	if (success)
	{
		ft_printf(COLOR_GREEN "  ✓ PASSED\n" COLOR_RESET);
		stats->passed++;
	}
	else
	{
		ft_printf(COLOR_RED "  ✗ FAILED\n" COLOR_RESET);
		stats->failed++;
	}
	gc_destroy(shell->cmd_arena);
	shell->cmd_arena = gc_create();

	// Test: Complex command
	stats->total++;
	ft_printf("\nTest %d: Complex \"(ls -la && cat f.txt) | grep test > out\"\n",
		stats->total);
	tokens = lexer("(ls -la && cat f.txt) | grep test > out", shell);
	print_token_list(tokens);
	if (tokens)
	{
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

/*
 * print_summary - Prints test summary
 */
void	print_summary(t_test_stats *stats)
{
	ft_printf("\n");
	ft_printf("========================================\n");
	ft_printf("           TEST SUMMARY\n");
	ft_printf("========================================\n");
	ft_printf("Total tests:  %d\n", stats->total);
	ft_printf(COLOR_GREEN "Passed:       %d\n" COLOR_RESET, stats->passed);
	if (stats->failed > 0)
		ft_printf(COLOR_RED "Failed:       %d\n" COLOR_RESET, stats->failed);
	else
		ft_printf("Failed:       %d\n", stats->failed);
	ft_printf("Success rate: %.1f%%\n",
		(float)stats->passed / stats->total * 100);
	ft_printf("========================================\n\n");
}

/*
 * main - Entry point for comprehensive lexer testing
 */
int	main(int ac, char **av, char **envp)
{
	t_shell			shell;
	t_test_stats	stats;

	(void)ac;
	(void)av;
	(void)envp;

	// Initialize
	stats.total = 0;
	stats.passed = 0;
	stats.failed = 0;

	shell.cmd_arena = gc_create();
	if (!shell.cmd_arena)
	{
		ft_printf("Error: Failed to create GC arena\n");
		return (1);
	}

	ft_printf("\n");
	ft_printf("========================================\n");
	ft_printf("   COMPREHENSIVE LEXER TEST SUITE\n");
	ft_printf("========================================\n");

	// Run test suites
	run_basic_test(&shell, &stats);
	run_operator_test(&shell, &stats);
	run_quote_test(&shell, &stats);
	run_edge_case_test(&shell, &stats);

	// Print summary
	print_summary(&stats);

	// Cleanup
	gc_destroy(shell.cmd_arena);

	return (stats.failed == 0 ? 0 : 1);
}
