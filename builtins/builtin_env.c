/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 19:19:57 by akivam            #+#    #+#             */
/*   Updated: 2025/12/09 18:25:56 by akivam           ###   ########.fr       */
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
		ft_err_printf("minishell: env: ");
		ft_err_printf("%s", args[1]);
		ft_err_printf(": No such file or directory\n");
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
	t_env	*current;

	if (args[1])
		return (execute_env_cmd(args, shell));
	if (!shell || !shell->env_list)
		return (1);
	current = shell->env_list;
	while (current)
	{
		if (current->value)
			ft_printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
	return (0);
}
