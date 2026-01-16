/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 00:38:11 by harici            #+#    #+#             */
/*   Updated: 2026/01/16 00:38:11 by harici           ###   ########.fr       */
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
