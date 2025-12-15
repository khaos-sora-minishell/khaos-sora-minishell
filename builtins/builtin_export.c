/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 19:19:57 by akivam            #+#    #+#             */
/*   Updated: 2025/12/15 21:09:09 by akivam           ###   ########.fr       */
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

/* Basit Bubble Sort (Diziyi sıralar) */
static void	sort_env_array(char **arr, int len)
{
	int		i;
	int		j;
	char	*swap;

	i = 0;
	while (i < len - 1)
	{
		j = 0;
		while (j < len - i - 1)
		{
			if (ft_strcmp(arr[j], arr[j + 1]) > 0)
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

/* Sıralı şekilde 'declare -x' formatında yazdır */
static void	print_sorted_env(t_shell *shell)
{
	char	**arr;
	int		len;
	int		i;
	char	*eq_pos;

	// Hash Table'ı diziye çevir (env_manager.c'deki fonksiyon)
	arr = env_table_to_array(shell->env_table, shell->cmd_arena);
	if (!arr) return;

	len = 0;
	while (arr[len])
		len++;
	
	sort_env_array(arr, len);

	i = 0;
	while (i < len)
	{
		// _ değişkenini yazdırma (bash davranışı)
		if (ft_strncmp(arr[i], "_=", 2) != 0)
		{
			// KEY=VALUE formatından ayırıp tırnak içine alarak yazdır
			eq_pos = ft_strchr(arr[i], '=');
			if (eq_pos)
			{
				*eq_pos = '\0'; // Eşittiri geçici olarak null yap
				printf("declare -x %s=\"%s\"\n", arr[i], eq_pos + 1);
				*eq_pos = '='; // Geri düzelt (gerçi cmd_arena'da olduğu için şart değil)
			}
			else
				printf("declare -x %s\n", arr[i]);
		}
		i++;
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
	
	// YENİ: env_set kullanıyoruz
	if (eq)
		env_set(shell->env_table, key, eq + 1, contex);
	else if (!env_get(shell->env_table, key, contex))
		env_set(shell->env_table, key, NULL, contex); // Değersiz değişken
		
	return (0);
}

int	builtin_export(char **args, t_shell *shell)
{
	int i;
	int ret;

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
	// Export yaptıktan sonra array'i güncellememiz şart
	shell->env_array = env_table_to_array(shell->env_table, shell->global_arena);
	return (ret);
}
