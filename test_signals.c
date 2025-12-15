/* ************************************************************************** */
/*                                                                            */
/*   test_signals.c - Signal handling tester                                 */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

void	test_global_variable(void)
{
	printf("\n=== TEST 1: Global Variable ===\n");

	printf("Initial value: %d\n", g_signal);
	g_signal = SIGINT;
	printf("After SIGINT: %d\n", g_signal);
	g_signal = SIGQUIT;
	printf("After SIGQUIT: %d\n", g_signal);
	g_signal = 0;
	printf("Reset to: %d\n", g_signal);

	printf("✓ Global variable works correctly\n");
}

void	test_signal_handler(void)
{
	printf("\n=== TEST 2: Signal Handler ===\n");
	printf("Testing signal_handler() function...\n");

	g_signal = 0;
	printf("Before: g_signal = %d\n", g_signal);

	signal_handler(SIGINT);
	printf("After signal_handler(SIGINT): g_signal = %d\n", g_signal);

	if (g_signal == SIGINT)
		printf("✓ signal_handler sets g_signal correctly\n");
	else
		printf("✗ FAILED: g_signal not set\n");
}

void	test_setup_signals(void)
{
	struct sigaction	sa;

	printf("\n=== TEST 3: setup_signals() ===\n");

	setup_signals();

	// Check SIGINT handler
	sigaction(SIGINT, NULL, &sa);
	if (sa.sa_handler == signal_handler)
		printf("✓ SIGINT handler set to signal_handler\n");
	else
		printf("✗ FAILED: SIGINT handler incorrect\n");

	if (sa.sa_flags & SA_RESTART)
		printf("✓ SA_RESTART flag set\n");
	else
		printf("✗ FAILED: SA_RESTART not set\n");

	// Check SIGQUIT handler
	sigaction(SIGQUIT, NULL, &sa);
	if (sa.sa_handler == SIG_IGN)
		printf("✓ SIGQUIT handler set to SIG_IGN\n");
	else
		printf("✗ FAILED: SIGQUIT handler incorrect\n");
}

void	test_child_signals(void)
{
	struct sigaction	sa;

	printf("\n=== TEST 4: setup_child_signals() ===\n");

	setup_child_signals();

	// Check SIGINT handler
	sigaction(SIGINT, NULL, &sa);
	if (sa.sa_handler == SIG_DFL)
		printf("✓ SIGINT handler set to SIG_DFL\n");
	else
		printf("✗ FAILED: SIGINT handler incorrect\n");

	// Check SIGQUIT handler
	sigaction(SIGQUIT, NULL, &sa);
	if (sa.sa_handler == SIG_DFL)
		printf("✓ SIGQUIT handler set to SIG_DFL\n");
	else
		printf("✗ FAILED: SIGQUIT handler incorrect\n");
}

void	test_ignore_signals(void)
{
	struct sigaction	sa;

	printf("\n=== TEST 5: ignore_signals() ===\n");

	ignore_signals();

	// Check SIGINT handler
	sigaction(SIGINT, NULL, &sa);
	if (sa.sa_handler == SIG_IGN)
		printf("✓ SIGINT handler set to SIG_IGN\n");
	else
		printf("✗ FAILED: SIGINT handler incorrect\n");

	// Check SIGQUIT handler
	sigaction(SIGQUIT, NULL, &sa);
	if (sa.sa_handler == SIG_IGN)
		printf("✓ SIGQUIT handler set to SIG_IGN\n");
	else
		printf("✗ FAILED: SIGQUIT handler incorrect\n");
}

void	test_interactive_behavior(void)
{
	printf("\n=== TEST 6: Interactive Behavior (Manual) ===\n");
	printf("Setting up interactive mode signals...\n");

	setup_signals();

	printf("\nPress Ctrl+C to test (or wait 3 seconds)...\n");
	printf("Expected: New line + prompt behavior\n");

	sleep(3);
	printf("✓ Signal setup complete (manual testing needed)\n");
}

int	main(int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;
	(void)envp;

	printf("========================================\n");
	printf("    SIGNAL HANDLING TEST SUITE\n");
	printf("========================================\n");

	test_global_variable();
	test_signal_handler();
	test_setup_signals();
	test_child_signals();
	test_ignore_signals();
	test_interactive_behavior();

	printf("\n========================================\n");
	printf("         ALL TESTS COMPLETED\n");
	printf("========================================\n");

	return (0);
}
