/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   harici_matrix_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 20:59:47 by akivam            #+#    #+#             */
/*   Updated: 2026/01/18 20:59:52 by akivam           ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "extra_commands.h"
#include "libft.h"
#include <readline/readline.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

static void	run_matrix_child(t_shell *shell)
{
	int				*drops;
	int				i;
	unsigned int	seed;

	seed = 42;
	drops = gc_malloc(shell->cmd_arena, sizeof(int) * MATRIX_WIDTH);
	if (!drops)
		exit(1);
	i = -1;
	while (++i < MATRIX_WIDTH)
		drops[i] = simple_rand(&seed) % 20;
	write(1, "\033[2J\033[?25l", 11);
	i = 0;
	while (i++ < 350 && get_signal() != SIGINT)
	{
		draw_rain(drops, MATRIX_WIDTH, MATRIX_HEIGHT, &seed);
		simple_delay();
	}
	write(1, "\033[0m\033[2J\033[H\033[?25h", 18);
	rl_clear_history();
	gc_destroy(shell->cmd_arena);
	gc_destroy(shell->global_arena);
	exit(0);
}

void	harici_matrix(char **args, t_shell *shell)
{
	pid_t	pid;
	int		status;

	(void)args;
	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		return ;
	}
	if (pid == 0)
		run_matrix_child(shell);
	else
	{
		ignore_signals();
		waitpid(pid, &status, 0);
		setup_signals();
		if (WIFEXITED(status))
			shell->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			shell->exit_status = 128 + WTERMSIG(status);
	}
}
