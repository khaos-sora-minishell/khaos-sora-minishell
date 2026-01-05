/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 03:59:58 by harici            #+#    #+#             */
/*   Updated: 2025/12/22 04:31:57 by harici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

char	*extract_var_name(char *str, int *i, t_shell *shell)
{
	int		start;
	int		len;
	char	*name;

	start = *i;
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	len = *i - start;
	if (len == 0)
		return (NULL);
	name = gc_malloc(shell->cmd_arena, len + 1);
	ft_memcpy(name, str + start, len);
	name[len] = '\0';
	return (name);
}

char	*expand_exit_status(t_shell *shell)
{
	return (gc_itoa(shell->cmd_arena, shell->exit_status));
}

char	*expand_arg_count(t_shell *shell)
{
	int	count;

	count = shell->argc - 1;
	if (count < 0)
		count = 0;
	return (gc_itoa(shell->cmd_arena, count));
}

char	*expand_positional_arg(t_shell *shell, int index)
{
	if (index < 0 || index >= shell->argc)
		return (gc_strdup(shell->cmd_arena, ""));
	return (gc_strdup(shell->cmd_arena, shell->argv[index]));
}

char	*expand_all_args(t_shell *shell)
{
	char	*result;
	char	*temp;
	int		i;

	if (shell->argc <= 1)
		return (gc_strdup(shell->cmd_arena, ""));
	result = gc_strdup(shell->cmd_arena, shell->argv[1]);
	i = 2;
	while (i < shell->argc)
	{
		temp = gc_strjoin(shell->cmd_arena, result, " ");
		result = gc_strjoin(shell->cmd_arena, temp, shell->argv[i]);
		i++;
	}
	return (result);
}

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

char	*process_dollar(char *str, int *i, t_shell *shell)
{
	char	*var_name;
	char	*value;

	(*i)++;
	if (str[*i] == '?')
	{
		(*i)++;
		return (expand_exit_status(shell));
	}
	if (str[*i] == '$')
	{
		(*i)++;
		return (gc_itoa(shell->cmd_arena, getpid()));
	}
	if (str[*i] == '#')
	{
		(*i)++;
		return (expand_arg_count(shell));
	}
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
