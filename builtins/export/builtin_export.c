/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 19:19:57 by akivam            #+#    #+#             */
/*   Updated: 2025/12/21 19:41:21 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "libft.h"
#include "minishell.h"
#include "printf.h"
#include "utils.h"

static int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static int	export_arg(char *arg, t_shell *shell)
{
	char			*eq_pos;
	char			*key;
	t_gc_context	*ctx;

	ctx = (t_gc_context *)shell->global_arena;
	eq_pos = ft_strchr(arg, '=');
	if (eq_pos)
		key = gc_strndup(ctx, arg, eq_pos - arg);
	else
		key = gc_strdup(ctx, arg);
	if (!is_valid_identifier(key))
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putendl_fd("': not a valid identifier", 2);
		return (1);
	}
	if (eq_pos)
		env_set(shell->env_table, key, eq_pos + 1, ctx);
	else
	{
		if (!env_get(shell->env_table, key, ctx))
			env_set(shell->env_table, key, NULL, ctx);
	}
	return (0);
}

int	builtin_export(char **args, t_shell *shell)
{
	int	i;
	int	ret;

	if (!args[1])
	{
		print_sorted_env(shell);
		return (0);
	}
	i = 1;
	ret = 0;
	while (args[i])
	{
		if (export_arg(args[i], shell) != 0)
			ret = 1;
		i++;
	}
	shell->env_array = env_table_to_array(shell->env_table,
			shell->global_arena);
	return (ret);
}
