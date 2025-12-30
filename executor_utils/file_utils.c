/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 20:41:02 by akivam            #+#    #+#             */
/*   Updated: 2025/12/07 20:41:31 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>

/*
		utility - read-only
 */
int	open_file(const char *file_name)
{
	int	fd;

	fd = open(file_name, O_RDONLY, 0644);
	if (fd < 0)
		return (-1);
	return (fd);
}

/*
		- write, create, truncate
 */
int	open_file_write(const char *file_name)
{
	int	fd;

	fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (-1);
	return (fd);
}

/*
		- append mode
 */
int	open_file_append(const char *file_name)
{
	int	fd;

	fd = open(file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		return (-1);
	return (fd);
}
