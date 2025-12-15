/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 19:19:57 by akivam            #+#    #+#             */
/*   Updated: 2025/12/15 21:09:09 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "libft.h"
#include "minishell.h"
#include "printf.h"

static int	execute_env_cmd(char **args, t_shell *shell)
{
	pid_t	pid;
	int		status;
	char	*path;

	path = find_command_path(args[1], shell);
	if (!path)
	{
		ft_err_printf("minishell: env: %s: No such file or directory\n", args[1]);
		return (127);
	}
	pid = fork();
	if (pid == 0)
	{
		execve(path, &args[1], shell->env_array);
		perror("minishell: env: execve");
		exit(126);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

int	builtin_env(char **args, t_shell *shell)
{
	int				i;
	t_env_bucket	*curr;
	char			*val_decoded;
	t_gc_context	*contex;

	if (args[1])
		return (execute_env_cmd(args, shell));
	if (!shell || !shell->env_table)
		return (1);
	
	contex = (t_gc_context *)shell->global_arena;
	i = 0;
	// Tüm Hash Table bucketlarını gez
	while (i < ENV_TABLE_SIZE)
	{
		curr = shell->env_table->buckets[i];
		while (curr)
		{
			// Değer varsa ve boş değilse yazdır
			if (curr->value)
			{
				// Şifreyi çözüp yazdır
				val_decoded = gc_strdup(contex, curr->value);
				xor_cipher(val_decoded);
				ft_printf("%s=%s\n", curr->key, val_decoded);
			}
			curr = curr->next;
		}
		i++;
	}
	return (0);
}
