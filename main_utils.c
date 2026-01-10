/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 21:13:51 by harici            #+#    #+#             */
/*   Updated: 2026/01/10 18:48:15 by harici           ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "executor.h"
#include "garbage_collector.h"
#include "lexer.h"
#include "libft.h"
#include "minishell.h"
#include <readline/history.h>
#include <stdio.h>
#include <unistd.h>

static pid_t	read_shell_pid(void)
{
	int		fd;
	char	buf[32];
	int		i;
	pid_t	pid;
	int		bytes_read;

	fd = open("/proc/self/stat", O_RDONLY);
	if (fd < 0)
		return (0);
	bytes_read = read(fd, buf, 31);
	close(fd);
	if (bytes_read <= 0)
		return (0);
	buf[bytes_read] = '\0';
	i = 0;
	pid = 0;
	while (buf[i] && buf[i] >= '0' && buf[i] <= '9')
	{
		pid = pid * 10 + (buf[i] - '0');
		i++;
	}
	return (pid);
}

static void	init_shell_common(t_shell *shell, int argc, char **argv,
		char **envp)
{
	ft_bzero(shell, sizeof(t_shell));
	shell->global_arena = gc_create();
	shell->cmd_arena = gc_create();
	if (!shell->global_arena || !shell->cmd_arena)
		exit(1);
	shell->env_table = initialize_env_table(envp, shell->global_arena);
	shell->env_array = env_table_to_array(shell->env_table,
			shell->global_arena);
	shell->alias_table = gc_calloc(shell->global_arena, 1, sizeof(t_env_table));
	shell->alias_table->buckets = gc_calloc(shell->global_arena,
			ENV_TABLE_SIZE, sizeof(t_env_bucket *));
	shell->terminal_name = "minishell";
	shell->terminal_name_color = NULL;
	shell->terminal_bg_color = NULL;
	shell->stdin_backup = -1;
	shell->stdout_backup = -1;
	shell->redir_stdin_backup = -1;
	shell->redir_stdout_backup = -1;
	shell->current_input = NULL;
	shell->argc = argc;
	shell->argv = argv;
	shell->shell_pid = read_shell_pid();
	shell->exit_status = 0;
}

#ifdef BONUS

void	init_shell(t_shell *shell, int argc, char **argv, char **envp)
{
	init_shell_common(shell, argc, argv, envp);
	init_history(shell);
	create_shellrc(shell);
}

#else

void	init_shell(t_shell *shell, int argc, char **argv, char **envp)
{
	init_shell_common(shell, argc, argv, envp);
}

#endif

void	clean_loop(t_shell *shell)
{
	gc_scope_pop_all(shell->cmd_arena);
	set_signal(0);
	shell->ast_root = NULL;
}
