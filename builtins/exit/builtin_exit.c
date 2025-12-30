/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 19:19:57 by akivam            #+#    #+#             */
/*   Updated: 2025/12/22 14:04:16 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "garbage_collector.h"
#include "libft.h"
#include "minishell.h"
#include "utils.h"
#include <stdlib.h>

static int	is_numeric(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	is_really_neg_one(char *str)
{
	int	i;

	i = 0;
	while (str[i] && is_whitespace(str[i]))
		i++;
	if (str[i] != '-')
		return (0);
	i++;
	while (str[i] == '0')
		i++;
	if (str[i] == '1' && !ft_isdigit(str[i + 1]))
		return (1);
	return (0);
}

static void	quit_shell(t_shell *shell, int status)
{
	cleanup_shell(shell);
	exit(status);
}

int	builtin_exit(char **args, t_shell *shell)
{
	ft_putendl_fd("exit", 2);
	if (!args[1])
		quit_shell(shell, shell->exit_status);
	if (!is_numeric(args[1]) || (ft_atoll(args[1]) == -1
			&& !is_really_neg_one(args[1])))
	{
		ft_err_printf("minishell: exit: %s: numeric argument required\n",
			args[1]);
		quit_shell(shell, 2);
	}
	if (args[2])
	{
		ft_err_printf("minishell: exit: too many arguments\n");
		return (1);
	}
	quit_shell(shell, (unsigned char)ft_atoll(args[1]));
	return (0);
}
