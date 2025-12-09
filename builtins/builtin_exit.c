/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 19:19:57 by akivam            #+#    #+#             */
/*   Updated: 2025/12/09 16:16:29 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "printf.h"
#include "garbage_collector.h"
#include <stdlib.h>

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

/*
** builtin_exit - Exit the shell
** Usage: exit [n]
** Exit with status n (0-255), or last exit status if no argument
*/
int	builtin_exit(char **args, t_shell *shell)
{
	int				exit_code;
	t_gc_context	*gc;

	ft_putendl_fd("exit", 1);
	gc = (t_gc_context *)shell->global_arena;
	if (!args[1])
	{
		gc_destroy(gc);
		exit(shell->exit_status);
	}
	if (!is_numeric(args[1]))
	{
		ft_err_printf("minishell: exit: %s: numeric argument required\n",
			args[1]);
		gc_destroy(gc);
		exit(2);
	}
	if (args[2])
		return (ft_err_printf("minishell: exit: too many arguments\n"), 1);
	exit_code = ft_atoi(args[1]) % 256;
	gc_destroy(gc);
	exit(exit_code);
}
