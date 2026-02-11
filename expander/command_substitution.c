/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_substitution.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 01:46:11 by akivam            #+#    #+#             */
/*   Updated: 2026/02/12 02:43:58 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

static char	*trim_trailing_newline(char *str, t_shell *shell)
{
	int	len;

	if (!str)
		return (gc_strdup(shell->cmd_arena, ""));
	len = ft_strlen(str);
	while (len > 0 && str[len - 1] == '\n')
		len--;
	return (gc_strndup(shell->cmd_arena, str, len));
}

char	*process_command_substitution(char *str, int *i, t_shell *shell)
{
	char	*command;
	char	*output;

	command = extract_command_from_parens(str, i, shell);
	if (!command || !*command)
		return (gc_strdup(shell->cmd_arena, ""));
	output = execute_command_in_subshell(command, shell);
	output = trim_trailing_newline(output, shell);
	return (output);
}
