/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_positional.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 12:57:52 by akivam            #+#    #+#             */
/*   Updated: 2026/01/11 12:57:52 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

#ifdef BONUS

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

#else

char	*expand_positional_arg(t_shell *shell, int index)
{
	(void)index;
	return (gc_strdup(shell->cmd_arena, ""));
}

char	*expand_all_args(t_shell *shell)
{
	return (gc_strdup(shell->cmd_arena, ""));
}

#endif
