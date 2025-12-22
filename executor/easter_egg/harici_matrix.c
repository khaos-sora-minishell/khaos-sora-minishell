/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   harici_matrix.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 13:25:00 by harici            #+#    #+#             */
/*   Updated: 2025/12/22 13:57:55 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "easteregg.h"
#include "libft.h"
#include <readline/readline.h> /* rl_clear_history için */
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

/* -- Helper Functions -- */

static unsigned int	simple_rand(unsigned int *seed)
{
	*seed = (*seed * 1103515245 + 12345) % 2147483648;
	return (*seed);
}

static void	simple_delay(void)
{
	int	i;
	int	j;

	i = 0;
	while (i < 50000)
	{
		j = 0;
		while (j < 100)
			j++;
		i++;
	}
}

static void	write_at(int row, int col, char c, int bright)
{
	char	buf[30];

	buf[0] = '\033';
	buf[1] = '[';
	buf[2] = '0' + (row / 10);
	buf[3] = '0' + (row % 10);
	buf[4] = ';';
	buf[5] = '0' + (col / 10);
	buf[6] = '0' + (col % 10);
	buf[7] = 'H';
	buf[8] = '\033';
	buf[9] = '[';
	if (bright)
		buf[10] = '1';
	else
		buf[10] = '2';
	buf[11] = ';';
	buf[12] = '3';
	buf[13] = '2';
	buf[14] = 'm';
	buf[15] = c;
	write(1, buf, 16);
}

static void	draw_rain(int *drops, int width, int height, unsigned int *seed)
{
	int		x;
	char	c;

	x = 0;
	while (x < width)
	{
		if (drops[x] >= height)
			drops[x] = 0;
		c = "01"[simple_rand(seed) % 2];
		write_at(drops[x], x + 1, c, 1);
		if (drops[x] > 0)
			write_at(drops[x] - 1, x + 1, c, 0);
		drops[x]++;
		x++;
	}
}

/* -- Child Process Mantığı -- */

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
	/* SOFT CLEANING: Çıkmadan önce child'ın hafızasını temizle */
	rl_clear_history();              /* Readline geçmişini temizle */
	gc_destroy(shell->cmd_arena);    /* Command arenayı temizle */
	gc_destroy(shell->global_arena); /* Global arenayı temizle */
	exit(0);
}

/* -- Ana Fonksiyon -- */

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
