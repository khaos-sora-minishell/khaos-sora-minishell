/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 19:19:57 by akivam            #+#    #+#             */
/*   Updated: 2025/12/22 12:46:14 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "garbage_collector.h"
#include "libft.h"
#include "minishell.h"

static int	parse_flags(char *arg, t_echo_contex *contex)
{
	int	i;

	if (!arg || arg[0] != '-')
		return (0);
	i = 1;
	while (arg[i])
	{
		if (arg[i] == 'n')
			contex->n = 1;
		else if (arg[i] == 'e')
			contex->e = 1;
		else if (arg[i] == 'u')
			contex->ops[0] = 1;
		else if (arg[i] == 'r')
			contex->ops[1] = 1;
		else
			return (0);
		i++;
	}
	if (i == 1)
		return (0);
	return (1);
}

static void	accept_echo_ops(char **str, t_echo_contex *contex)
{
	static const t_echo_op	operations[] = {
	{'u', op_upper},
	{'r', op_reverse},
	{0, NULL}};
	int						i;

	i = 0;
	while (operations[i].func)
	{
		if (contex->ops[i] == 1)
			operations[i].func(*str);
		i++;
	}
}

static void	echo_print_arg(char **args, int i, t_echo_contex *contex)
{
	char	*temp;

	while (args[i])
	{
		temp = gc_strdup(contex->garbage_collector_contex, args[i]);
		accept_echo_ops(&temp, contex);
		if (contex->e)
			print_with_escape(temp);
		else
			ft_putstr_fd(temp, 1);
		if (args[i + 1])
			write(1, " ", 1);
		i++;
	}
}

int	builtin_echo(char **args, t_shell *shell)
{
	t_echo_contex	contex;
	int				i;

	contex.garbage_collector_contex = (t_gc_context *)shell->global_arena;
	contex.n = 0;
	contex.e = 0;
	contex.ops[0] = 0;
	contex.ops[1] = 0;
	i = 1;
	while (args[i] && parse_flags(args[i], &contex))
		i++;
	echo_print_arg(args, i, &contex);
	if (!contex.n)
		write(1, "\n", 1);
	return (0);
}
