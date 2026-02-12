/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unalias.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 19:07:12 by akivam            #+#    #+#             */
/*   Updated: 2025/12/22 19:07:19 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "shell.h"

int	builtin_unalias(char **args, t_shell *shell)
{
	int	i;

	if (!args[1])
	{
		ft_putendl_fd("minishell: unalias: too few arguments", 2);
		return (1);
	}
	i = 1;
	while (args[i])
	{
		env_unset(shell->alias_table, args[i]);
		i++;
	}
	return (0);
}
