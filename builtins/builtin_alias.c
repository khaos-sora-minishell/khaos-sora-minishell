/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_alias.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 18:57:08 by akivam            #+#    #+#             */
/*   Updated: 2025/12/22 19:10:04 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

static int	get_alias_key_len(char *arg)
{
	char	*equal_pos;

	equal_pos = ft_strchr(arg, '=');
	if (!equal_pos)
		return (-1);
	return (equal_pos - arg);
}

static void	print_all_aliases(t_shell *shell)
{
	int				i;
	t_env_bucket	*node;
	char			*decrypted_val;

	i = -1;
	while (++i < ENV_TABLE_SIZE)
	{
		node = shell->alias_table->buckets[i];
		while (node)
		{
			if (node->value)
			{
				decrypted_val = env_get(shell->alias_table, node->key,
						shell->cmd_arena);
				if (decrypted_val)
					ft_printf("alias %s='%s'\n", node->key, decrypted_val);
			}
			node = node->next;
		}
	}
}

static int	process_alias_arg(char *arg, t_shell *shell)
{
	int		key_len;
	char	*key;
	char	*value;

	key_len = get_alias_key_len(arg);
	if (key_len != -1)
	{
		key = gc_substr(shell->cmd_arena, arg, 0, key_len);
		value = gc_strdup(shell->cmd_arena, arg + key_len + 1);
		env_set(shell->alias_table, key, value, shell->global_arena);
		return (0);
	}
	value = env_get(shell->alias_table, arg, shell->cmd_arena);
	if (value)
	{
		ft_printf("alias %s='%s'\n", arg, value);
		return (0);
	}
	ft_putstr_fd("minishell: alias: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putendl_fd(": not found", 2);
	return (1);
}

int	builtin_alias(char **args, t_shell *shell)
{
	int	i;
	int	ret;

	if (!args[1])
	{
		print_all_aliases(shell);
		return (0);
	}
	ret = 0;
	i = 1;
	while (args[i])
	{
		if (process_alias_arg(args[i], shell) != 0)
			ret = 1;
		i++;
	}
	return (ret);
}
