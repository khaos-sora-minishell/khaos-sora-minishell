/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 19:19:57 by akivam            #+#    #+#             */
/*   Updated: 2025/12/11 18:34:25 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "garbage_collector.h"
#include "libft.h"
#include "minishell.h"
#include "printf.h"
#include <stdlib.h>
#include "utils.h"

/*
** Check if string is a valid number
*/
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
/*
** builtin_exit - Exit the shell
** Usage: exit [n]
** Exit with status n (0-255), or last exit status if no argument
*/
int	builtin_exit(char **args, t_shell *shell)
{
	long long		exit_code;
	t_gc_context	*contex;

	ft_putendl_fd("exit", 2);
	contex = (t_gc_context *)shell->global_arena;
	if (!args[1])
	{
		gc_destroy(contex);
		exit(shell->exit_status);
	}
	if (!is_numeric(args[1]))
	{
		ft_err_printf("minishell: exit: %s: numeric argument required\n",
			args[1]);
		gc_destroy(contex);
		exit(2);
	}
	exit_code = ft_atoll(args[1]);
	if (exit_code == -1 && !is_really_neg_one(args[1]))
	{
		ft_err_printf("minishell: exit: %s: numeric argument required\n",
			args[1]);
		gc_destroy(contex);
		exit(2);
	}
	if (args[2])
		return (ft_err_printf("minishell: exit: too many arguments\n"), 1);
	gc_destroy(contex);
	exit((unsigned char)exit_code);
}
