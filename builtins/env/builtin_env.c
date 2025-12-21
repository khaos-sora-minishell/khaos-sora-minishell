/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 19:19:57 by akivam            #+#    #+#             */
/*   Updated: 2025/12/21 19:06:24 by akivam           ###   ########.fr       */
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
		ft_err_printf("minishell: env: %s: No such file or directory\n",
			args[1]);
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

static void	print_env_bucket(t_env_bucket *current, t_shell *shell)
{
	char			*decoded_value;
	t_gc_context	*contex;

	contex = (t_gc_context *)shell->global_arena;
	while (current)
	{
		if (current->_has_value == 1)
		{
			decoded_value = gc_strdup(contex, current->value);
			xor_cipher(decoded_value);
			ft_printf("%s=%s\n", current->key, decoded_value);
		}
		current = current->next;
	}
}

int	builtin_env(char **args, t_shell *shell)
{
	int	i;

	if (args[1])
		return (execute_env_cmd(args, shell));
	if (!shell || !shell->env_table)
		return (1);
	i = -1;
	while (++i < ENV_TABLE_SIZE)
	{
		if (shell->env_table->buckets[i])
			print_env_bucket(shell->env_table->buckets[i], shell);
	}
	return (0);
}
