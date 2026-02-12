/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 20:28:40 by harici            #+#    #+#             */
/*   Updated: 2026/01/13 20:28:40 by harici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "shell.h"

char	*expand_variable(char *var_name, t_shell *shell)
{
	char	*value;

	if (!var_name)
		return (gc_strdup(shell->cmd_arena, ""));
	value = env_get(shell->env_table, var_name, shell->cmd_arena);
	if (!value)
		return (gc_strdup(shell->cmd_arena, ""));
	return (value);
}

char	*process_simple_special(char *str, int *i, t_shell *shell)
{
	if (str[*i] == '?')
	{
		(*i)++;
		return (expand_exit_status(shell));
	}
	if (str[*i] == '$')
	{
		(*i)++;
		return (gc_itoa(shell->cmd_arena, shell->shell_pid));
	}
	if (str[*i] == '#')
	{
		(*i)++;
		return (expand_arg_count(shell));
	}
	return (NULL);
}

char	*process_arg_special(char *str, int *i, t_shell *shell)
{
	char	*value;

	if (str[*i] == '@' || str[*i] == '*')
	{
		(*i)++;
		return (expand_all_args(shell));
	}
	if (ft_isdigit(str[*i]))
	{
		value = expand_positional_arg(shell, str[*i] - '0');
		(*i)++;
		return (value);
	}
	return (NULL);
}
