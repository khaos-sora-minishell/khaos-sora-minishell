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
