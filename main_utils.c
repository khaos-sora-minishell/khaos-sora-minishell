/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 12:06:42 by akivam            #+#    #+#             */
/*   Updated: 2026/02/12 02:42:09 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "garbage_collector.h"
#include "lexer.h"
#include "libft.h"
#include "minishell.h"
#include <readline/history.h>
#include <stdio.h>
#include <unistd.h>

pid_t	read_shell_pid(void)
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

char	*read_hostname(void *arena)
{
	int		fd;
	char	buf[256];
	int		bytes;
	int		i;

	fd = open("/etc/hostname", O_RDONLY);
	if (fd < 0)
		return (NULL);
	bytes = read(fd, buf, 255);
	close(fd);
	if (bytes <= 0)
		return (NULL);
	buf[bytes] = '\0';
	i = 0;
	while (buf[i] && buf[i] != '\n')
		i++;
	buf[i] = '\0';
	return (gc_strdup(arena, buf));
}

static char	*get_user_hostname(void *arena)
{
	char	*hostname;
	char	*user;
	char	*result;

	user = getenv("USER");
	if (!user)
		user = "user";
	hostname = getenv("HOSTNAME");
	if (!hostname)
		hostname = getenv("HOST");
	if (!hostname)
		hostname = read_hostname(arena);
	result = gc_strdup(arena, user);
	if (hostname)
	{
		result = gc_strjoin(arena, result, "@");
		result = gc_strjoin(arena, result, hostname);
	}
	return (result);
}

char	*get_default_terminal_name(void *arena)
{
	char	*result;
	char	*cwd;
	char	*home;

	result = get_user_hostname(arena);
	result = gc_strjoin(arena, result, ":");
	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		gc_track((t_gc_context *)arena, cwd);
		home = getenv("HOME");
		if (home && ft_strncmp(cwd, home, ft_strlen(home)) == 0)
		{
			result = gc_strjoin(arena, result, "~");
			result = gc_strjoin(arena, result, cwd + ft_strlen(home));
		}
		else
			result = gc_strjoin(arena, result, cwd);
	}
	else
		result = gc_strjoin(arena, result, "?");
	return (result);
}
