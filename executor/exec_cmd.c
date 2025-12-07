/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 19:20:37 by akivam            #+#    #+#             */
/*   Updated: 2025/11/19 19:20:37 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "executor.h"
#include "minishell.h"
#include "builtins.h"

/*
find executable path for command
returns command path or null if not find 
*/
char *find_command_path(char *cmd, t_shell *shell)
{
	int i;
	char *path;
	char *full_path;
	t_gc_context *contex;
	if(!cmd || shell->path_dirs)
		return NULL;
	if (ft_strchr(cmd, '/'))
	{
		if(access(cmd, X_OK) == 0)
			return(cmd);
		return NULL;
	}
	contex = gc_get_current();
	i = 0;
	while (shell->path_dirs[i])
	{
		path = gc_strjoin(contex, shell->path_dirs[i], "/");
		full_path = gc_strjoin(contex, path, cmd);
		if(access(full_path, X_OK) == 0)
			return(full_path);
		i++;
	}
	return NULL;
}

/*
 * Tek bir komutu çalıştırır - fork, execve
 */
void	execute_command(t_cmd *cmd, t_shell *shell)
{
	pid_t	pid;
	char	*cmd_path;
	int		status;

	if (!cmd || !cmd->args || !cmd->args[0])
		return ;
	// builtin kontrolü
	if (is_builtin(cmd->args[0]))
	{
		shell->exit_status = execute_builtin(cmd->args, shell);
		return ;
	}
	// external cmd için fork
	pid = fork();
	if (pid == 0)
	{
		// setup redirections
		if (setup_redirections(cmd->redirs, shell) == -1)
			exit(1);
		cmd_path = find_command_path(cmd->args[0], shell);
		if (!cmd_path)
		{
			ft_putstr_fd(cmd->args[0], 2);
			ft_putendl_fd(": command not found", 2);
			exit(127);
		}
		// execve
		execve(cmd_path, cmd->args, shell->env_array);
		// if execve faild
		perror("execve");
		exit(126);
	}
	// parent process
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
}
