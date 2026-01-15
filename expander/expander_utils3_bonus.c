/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils3_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 20:28:47 by harici            #+#    #+#             */
/*   Updated: 2026/01/15 00:00:00 by harici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

static char	*process_special_param(char *str, int *i, t_shell *shell)
{
	char	*value;

	value = process_simple_special(str, i, shell);
	if (value)
		return (value);
	value = process_arg_special(str, i, shell);
	if (value)
		return (value);
	return (NULL);
}

static char	*process_brace_var(char *str, int *i, t_shell *shell)
{
	char	*var_name;
	char	*value;

	(*i)++;
	var_name = extract_var_name(str, i, shell);
	if (str[*i] == '}')
		(*i)++;
	value = expand_variable(var_name, shell);
	return (value);
}

char	*process_dollar(char *str, int *i, t_shell *shell)
{
	char	*var_name;
	char	*value;

	(*i)++;
	value = process_special_param(str, i, shell);
	if (value)
		return (value);
	if (str[*i] == '"' || str[*i] == '\'')
		return (gc_strdup(shell->cmd_arena, ""));
	if (str[*i] == '{')
		return (process_brace_var(str, i, shell));
	if (!str[*i] || (!ft_isalnum(str[*i]) && str[*i] != '_'))
		return (gc_strdup(shell->cmd_arena, "$"));
	var_name = extract_var_name(str, i, shell);
	value = expand_variable(var_name, shell);
	return (value);
}

char	*process_tilde(char *str, int *i, t_shell *shell)
{
	char	*home;

	(*i)++;
	if (!str[*i] || str[*i] == '/')
	{
		home = env_get(shell->env_table, "HOME", shell->cmd_arena);
		if (!home)
			return (gc_strdup(shell->cmd_arena, "~"));
		return (home);
	}
	(*i)--;
	return (gc_strdup(shell->cmd_arena, "~"));
}
