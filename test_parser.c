/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 00:00:00 by harici            #+#    #+#             */
/*   Updated: 2025/12/19 00:00:00 by harici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"
#include "parser.h"
#include <stdio.h>

void	print_tokens(t_token *tokens)
{
	t_token	*current;
	int		i;
	const char *type_names[] = {
		"WORD", "PIPE", "REDIR_IN", "REDIR_OUT",
		"REDIR_APPEND", "HEREDOC", "AND", "OR",
		"LPAREN", "RPAREN"
	};

	printf("\n=== TOKENS ===\n");
	current = tokens;
	i = 0;
	while (current)
	{
		printf("[%d] Type: %-12s Value: '%s'\n",
			i++, type_names[current->type], current->value);
		current = current->next;
	}
	printf("\n");
}

void	print_cmd(t_cmd *cmd)
{
	int		i;
	t_redir	*redir;
	const char *redir_names[] = {
		"WORD", "PIPE", "IN", "OUT", "APPEND", "HEREDOC"
	};

	if (!cmd)
	{
		printf("Command is NULL!\n");
		return ;
	}
	printf("\n=== PARSED COMMAND ===\n");
	printf("Arguments:\n");
	if (cmd->args)
	{
		i = 0;
		while (cmd->args[i])
		{
			printf("  [%d] '%s'\n", i, cmd->args[i]);
			i++;
		}
	}
	else
		printf("  (no arguments)\n");
	printf("\nRedirections:\n");
	if (cmd->redirs)
	{
		redir = cmd->redirs;
		i = 0;
		while (redir)
		{
			printf("  [%d] Type: %-8s File: '%s'\n",
				i++, redir_names[redir->type], redir->file);
			redir = redir->next;
		}
	}
	else
		printf("  (no redirections)\n");
	printf("\n");
}

void	test_case(char *input, t_shell *shell)
{
	t_token	*tokens;
	t_cmd	*cmd;

	printf("\n========================================\n");
	printf("Testing: \"%s\"\n", input);
	printf("========================================\n");

	tokens = lexer(input, shell);
	if (!tokens)
	{
		printf("ERROR: Lexer returned NULL\n");
		return ;
	}

	print_tokens(tokens);

	cmd = parse_simple_command(&tokens, shell);
	print_cmd(cmd);

	printf("Remaining tokens after parsing:\n");
	if (tokens)
		print_tokens(tokens);
	else
		printf("  (none - all tokens consumed)\n");
}

int	main(void)
{
	t_shell	shell;
	void	*gc;

	gc = gc_create();
	if (!gc)
	{
		printf("Failed to create garbage collector\n");
		return (1);
	}

	shell.cmd_arena = gc;
	shell.global_arena = gc;
	shell.exit_status = 0;

	printf("\n");
	printf("╔════════════════════════════════════════╗\n");
	printf("║     PARSER TEST SUITE - HARICI         ║\n");
	printf("╔════════════════════════════════════════╗\n");

	test_case("ls -la", &shell);
	test_case("cat < input.txt", &shell);
	test_case("echo hello > output.txt", &shell);
	test_case("grep test < file.txt > result.txt", &shell);
	test_case("cat << EOF", &shell);
	test_case("ls -la >> log.txt", &shell);
	test_case("pwd", &shell);
	test_case("echo hello world test", &shell);

	printf("\n");
	printf("╔════════════════════════════════════════╗\n");
	printf("║          ALL TESTS COMPLETED           ║\n");
	printf("╔════════════════════════════════════════╗\n");
	printf("\n");

	gc_destroy(gc);
	return (0);
}
