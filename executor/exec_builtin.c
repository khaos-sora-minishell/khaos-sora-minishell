/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 22:21:32 by akivam            #+#    #+#             */
/*   Updated: 2025/12/12 17:14:52 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "libft.h"
#include "utils.h"

/*
** Returns the list of builtin commands
** Static function - not global!
*/
static t_builtin	*get_builtins(void)
{
	static t_builtin	builtins[] = {
	{"cd", builtin_cd},
	{"echo", builtin_echo},
	{"env", builtin_env},
	{"exit", builtin_exit},
	{"export", builtin_export},
	{"pwd", builtin_pwd},
	{"unset", builtin_unset},
	{NULL, NULL}};

	return (builtins);
}

/*
** Checks if the command is a builtin
*/
int	is_builtin(char *cmd)
{
	t_builtin	*builtins;
	int			i;

	if (!cmd)
		return (0);
	builtins = get_builtins();
	i = 0;
	while (builtins[i].name)
	{
		if (ft_strcmp(cmd, builtins[i].name) == 0)
			return (1);
		i++;
	}
	return (0);
}

/*
** Executes a builtin command
** No cmd parameter - taken from args[0]
*/
int	execute_builtin(char **args, t_shell *shell)
{
	t_builtin	*builtins;
	int			i;

	if (!args || !args[0])
		return (1);
	builtins = get_builtins();
	i = 0;
	while (builtins[i].name)
	{
		if (ft_strcmp(args[0], builtins[i].name) == 0)
			return (builtins[i].func(args, shell));
		i++;
	}
	return (1);
}
