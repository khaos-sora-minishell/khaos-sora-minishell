/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 19:19:57 by akivam            #+#    #+#             */
/*   Updated: 2025/12/12 14:22:13 by akivam           ###   ########.fr       */
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

static void	bubble_sort_env(t_env **arr, int len)
{
	int		i;
	int		j;
	t_env	*swap;

	i = 0;
	while (i < len - 1)
	{
		j = 0;
		while (j < len - i - 1)
		{
			if (ft_strcmp(arr[j]->key, arr[j + 1]->key) > 0)
			{
				swap = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = swap;
			}
			j++;
		}
		i++;
	}
}

static void	print_sorted_env_list(t_shell *shell)
{
	t_env	**arr;
	t_env	*tmp;
	int		len;
	int		i;

	len = 0;
	tmp = shell->env_list;
	while (tmp && ++len)
		tmp = tmp->next;
	arr = gc_malloc(shell->cmd_arena, sizeof(t_env *) * (len + 1));
	if (!arr)
		return ;
	tmp = shell->env_list;
	i = 0;
	while (tmp)
	{
		arr[i++] = tmp;
		tmp = tmp->next;
	}
	bubble_sort_env(arr, len);
	i = -1;
	while (++i < len)
	{
		if (ft_strcmp(arr[i]->key, "_") != 0)
		{
			if (arr[i]->value)
				printf("declare -x %s=\"%s\"\n", arr[i]->key, arr[i]->value);
			else
				printf("declare -x %s\n", arr[i]->key);
		}
	}
}

static int	export_arg(char *arg, t_shell *shell)
{
	char			*eq;
	char			*key;
	t_gc_context	*contex;

	contex = (t_gc_context *)shell->global_arena;
	eq = ft_strchr(arg, '=');
	if (eq)
		key = gc_strndup(contex, arg, eq - arg);
	else
		key = gc_strdup(contex, arg);
	if (!is_valid_identifier(key))
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putendl_fd("': not a valid identifier", 2);
		return (1);
	}
	if (eq)
	{
		set_env_value(&shell->env_list, key, gc_strdup(contex, eq + 1), contex);
	}
	else if (!get_env_value(shell->env_list, key))
	{
		set_env_value(&shell->env_list, key, NULL, contex);
	}
	return (0);
}

int	builtin_export(char **args, t_shell *shell)
{
	int i;
	int ret;

	if (!args[1])
	{
		print_sorted_env_list(shell);
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
	return (ret);
}