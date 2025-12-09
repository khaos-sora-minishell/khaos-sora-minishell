/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_help.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 21:22:31 by akivam            #+#    #+#             */
/*   Updated: 2025/12/09 21:22:48 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_help(char **args, t_shell *shell)
{
	(void)args;
	(void)shell;
	printf("\n🐚 MINISHELL HELP 🐚\n");
	printf("---------------------\n");
	printf("Built-in Commands:\n");
	printf("  cd [dir]       : Change directory\n");
	printf("  echo [-n] [arg]: Write arguments to stdout\n");
	printf("  env            : Print environment variables\n");
	printf("  exit [status]  : Exit the shell\n");
	printf("  export [var]   : Set environment variable\n");
	printf("  pwd            : Print working directory\n");
	printf("  unset [var]    : Unset environment variable\n");
	printf("  type [name]    : Display information about command type\n");
	printf("\nUse 'man bash' for more information about shell features.\n");
	return (0);
}
