/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 16:02:50 by akivam            #+#    #+#             */
/*   Updated: 2025/10/12 18:22:08 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "libft.h"

static char	*extract_line(char *stash)
{
	char	*newline;
	size_t	i;

	i = 0;
	if (!stash || stash[0] == '\0')
		return (NULL);
	newline = ft_strchr(stash, '\n');
	if (newline)
		i = (newline - stash) + 1;
	else
		i = ft_strlen(stash);
	return (ft_substr(stash, 0, i));
}

static char	*update_stash(char *stash)
{
	char	*new_stash;
	char	*new_line;

	if (!stash)
		return (NULL);
	new_line = ft_strchr(stash, '\n');
	if (!new_line)
	{
		free(stash);
		return (NULL);
	}
	if (*(new_line + 1))
		new_stash = ft_strdup(new_line + 1);
	else
		new_stash = NULL;
	free(stash);
	return (new_stash);
}

static char	*read_line(int fd, char *stash)
{
	char	*buffer;
	ssize_t	read_byte;

	buffer = ft_calloc(BUFFER_SIZE + 1, 1);
	if (!buffer)
		return (NULL);
	while (!stash || !ft_strchr(stash, '\n'))
	{
		read_byte = read(fd, buffer, BUFFER_SIZE);
		if (read_byte == -1)
		{
			free(buffer);
			free(stash);
			stash = NULL;
			return (NULL);
		}
		if (read_byte == 0)
			break ;
		buffer[read_byte] = '\0';
		stash = gnl_ft_strjoin(stash, buffer);
	}
	free(buffer);
	return (stash);
}

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
	{
		if (stash)
		{
			free(stash);
			stash = NULL;
		}
		return (NULL);
	}
	stash = read_line(fd, stash);
	if (!stash)
		return (NULL);
	line = extract_line(stash);
	stash = update_stash(stash);
	return (line);
}
