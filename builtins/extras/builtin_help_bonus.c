/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_help.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 21:22:31 by akivam            #+#    #+#             */
/*   Updated: 2026/01/06 22:19:56 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_help_commands(void)
{
	printf("\n🐚 MINISHELL HELP 🐚\n");
	printf("========================\n\n");
	printf("📋 Built-in Commands:\n");
	printf("  cd [dir]         : Change directory\n");
	printf("  echo [-n] [arg]  : Write arguments to stdout\n");
	printf("  env              : Print environment variables\n");
	printf("  exit [status]    : Exit the shell\n");
	printf("  export [var]     : Set environment variable\n");
	printf("  pwd              : Print working directory\n");
	printf("  unset [var]      : Unset environment variable\n\n");
	printf("🔧 Extra Commands:\n");
	printf("  type [name]      : Display information about command type\n");
	printf("  tty              : Print terminal device name\n");
	printf("  clear            : Clear the terminal screen\n");
	printf("  true             : Return success (exit code 0)\n");
	printf("  false            : Return failure (exit code 1)\n");
	printf("  help             : Display this help message\n");
	printf("  alias [name=cmd] : Create command alias\n");
	printf("  unalias [name]   : Remove command alias\n");
}

static void	print_help_footer(void)
{
	printf("\n🎉 Easter Eggs:\n");
	printf("  harici_matrix      : Display matrix effect\n");
	printf("  akivam_executer    : Show executor visualization\n");
	printf("  set-terminal [name]: Set terminal title\n");
	printf("\n💡 Tips:\n");
	printf("  - Use Ctrl+D (EOF) to exit\n");
	printf("  - Aliases are loaded from ~/.shellrc\n");
	printf("  - Use 'man bash' for more shell features\n\n");
}

int	builtin_help(char **args, t_shell *shell)
{
	(void)args;
	(void)shell;
	print_help_commands();
	print_help_footer();
	return (0);
}
