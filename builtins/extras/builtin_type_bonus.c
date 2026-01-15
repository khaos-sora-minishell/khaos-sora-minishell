/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_type.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 21:12:06 by akivam            #+#    #+#             */
/*   Updated: 2026/01/06 22:10:04 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "minishell.h"
#include "printf.h"

int	builtin_type(char **args, t_shell *shell)
{
	char	*path;
	int		i;

	if (!args[1])
		return (0);
	i = 1;
	while (args[i])
	{
		if (is_builtin(args[i]))
			printf("%s is a shell builtin\n", args[i]);
		else
		{
			path = find_command_path(args[i], shell);
			if (path)
			{
				printf("%s is %s\n", args[i], path);
			}
			else
				ft_err_printf("minishell: type: %s: not found\n", args[i]);
		}
		i++;
	}
	return (0);
}
