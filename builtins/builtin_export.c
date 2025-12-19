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

/* ** Identifier Kontrolü 
** (Sayı ile başlayamaz, alfanümerik veya _ olmalı)
*/
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

/* ** Bucket Dizisini Sırala (Bubble Sort)
*/
static void	sort_buckets(t_env_bucket **arr, int count)
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

/*
** Tek bir değişkeni bash formatında yazdırır
** - Değer varsa: declare -x KEY="VALUE" (Value içindeki " ve \ kaçışlı)
** - Değer yoksa: declare -x KEY
*/
static void	print_export_item(t_env_bucket *item, t_shell *shell)
{
	char	*decrypted;
	int		i;

	if (ft_strncmp(item->key, "_", 2) == 0) // '_' değişkeni gizlenmeli
		return ;
	ft_printf("declare -x %s", item->key);
	if (item->_has_value)
	{
		ft_printf("=\"");
		// Değeri şifreden çöz
		decrypted = gc_strdup((t_gc_context *)shell->cmd_arena, item->value);
		xor_cipher(decrypted);
		
		// Özel karakterleri escape et (\ ve ")
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

/*
** Tüm tabloyu gez, diziye topla, sırala ve yazdır
*/
static void	print_sorted_env(t_shell *shell)
{
	t_env_bucket	**arr;
	t_env_bucket	*curr;
	int				count;
	int				i;
	int				j;

	if (!shell->env_table || shell->env_table->count == 0)
		return ;
	
	// Tablo boyutunda (veya count kadar) yer ayır
	arr = gc_malloc((t_gc_context *)shell->cmd_arena, 
			sizeof(t_env_bucket *) * (shell->env_table->count + 1));
	
	// Hash table'ı düz diziye dök
	count = 0;
	i = 0;
	while (i < ENV_TABLE_SIZE)
	{
		curr = shell->env_table->buckets[i];
		while (curr)
		{
			arr[count++] = curr;
			curr = curr->next;
		}
		i++;
	}

	sort_buckets(arr, count);

	// Yazdır
	j = 0;
	while (j < count)
	{
		print_export_item(arr[j], shell);
		j++;
	}
}

/*
** Argümanı işle ve env_set'i çağır
** - export VAR   -> has_value=0
** - export VAR=  -> has_value=1, value=""
** - export VAR=X -> has_value=1, value="X"
*/
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
	{
		// Eşittir var: Değer ata (boş string bile olsa değerlidir)
		env_set(shell->env_table, key, eq_pos + 1, ctx);
	}
	else
	{
		// Eşittir yok: Sadece değeri yoksa ekle, varsa dokunma
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
	// Export sonrası env array'i güncellemek kritik (child processler için)
	shell->env_array = env_table_to_array(shell->env_table, shell->global_arena);
	return (ret);
}
