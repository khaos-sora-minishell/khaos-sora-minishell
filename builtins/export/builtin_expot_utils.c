/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_expot_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 19:17:15 by akivam            #+#    #+#             */
/*   Updated: 2025/12/21 19:22:24 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "libft.h"
#include "minishell.h"
#include "printf.h"
#include "utils.h"

void	sort_buckets(t_env_bucket **arr, int count)
{
	int				i;
	int				j;
	t_env_bucket	*temp;

	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (ft_strcmp(arr[j]->key, arr[j + 1]->key) > 0)
			{
				temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}

void	print_export_item(t_env_bucket *item, t_shell *shell)
{
	char	*decrypted;
	int		i;

	if (ft_strncmp(item->key, "_", 2) == 0)
		return ;
	ft_printf("declare -x %s", item->key);
	if (item->_has_value)
	{
		ft_printf("=\"");
		decrypted = gc_strdup((t_gc_context *)shell->cmd_arena, item->value);
		xor_cipher(decrypted);
		i = 0;
		while (decrypted[i])
		{
			if (decrypted[i] == '\"' || decrypted[i] == '\\')
				ft_putchar_fd('\\', 1);
			ft_putchar_fd(decrypted[i], 1);
			i++;
		}
		ft_printf("\"");
	}
	ft_printf("\n");
}

void	print_sorted_env(t_shell *shell)
{
	t_env_bucket	**arr;
	t_env_bucket	*curr;
	int				count;
	int				i;
	int				j;

	if (!shell->env_table || shell->env_table->count == 0)
		return ;
	arr = gc_malloc((t_gc_context *)shell->cmd_arena, sizeof(t_env_bucket *)
			* (shell->env_table->count + 1));
	count = 0;
	i = -1;
	while (++i < ENV_TABLE_SIZE)
	{
		curr = shell->env_table->buckets[i];
		while (curr)
		{
			arr[count++] = curr;
			curr = curr->next;
		}
	}
	sort_buckets(arr, count);
	j = -1;
	while (++j < count)
		print_export_item(arr[j], shell);
}
