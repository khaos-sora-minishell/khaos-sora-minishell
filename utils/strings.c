/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strings.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 16:54:02 by harici            #+#    #+#             */
/*   Updated: 2025/12/21 21:37:25 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include "utils.h"
#include <fcntl.h>
#include <unistd.h>

int	open_file(const char *file_name)
{
	int	fd;

	fd = open(file_name, O_RDONLY, 0644);
	if (fd < 0)
		return (-1);
	return (fd);
}

int	open_file_write(const char *file_name)
{
	int	fd;

	fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (-1);
	return (fd);
}

int	open_file_append(const char *file_name)
{
	int	fd;

	fd = open(file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		return (-1);
	return (fd);
}
