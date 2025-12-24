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
** Builtin listesini döndürür
** Fonksiyon içi static - global değil!
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
	{"alias", builtin_alias},
	{"help", builtin_help},
	{"unalias", builtin_unalias},
	{"type", builtin_type},
	{"tty", builtin_tty},
	{"true", builtin_true},
	{"false", builtin_false},
	{NULL, NULL}};

	return (builtins);
}

/*
** Komutun builtin olup olmadığını kontrol eder
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
** Builtin komutunu çalıştırır
** cmd parametresi yok - args[0]'dan alınır
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
