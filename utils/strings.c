/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strings.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 00:00:00 by akivam            #+#    #+#             */
/*   Updated: 2025/12/15 21:40:57 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "utils.h"
#include <fcntl.h>
#include <unistd.h>

/*
 * Check if character is whitespace (comprehensive version from executor)
 */
int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v'
		|| c == '\f');
}

/*
 * Check if character is special shell character
 */
int	is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '&' || c == '('
		|| c == ')');
}

/*
 * String helper functions
 */
int	is_metachar(char c)
{
	return (is_special_char(c) || c == '\'' || c == '\"');
}

/*
 * String comparison
 */
int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while ((s1[i] != '\0' || s2[i] != '\0'))
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i = i + 1;
	}
	return (0);
}

/*
 * Convert string to long long
 */
long long	ft_atoll(const char *str)
{
	long long	res;
	int			sign;
	int			i;

	sign = 1;
	res = 0;
	i = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (res > LLONG_MAX / 10 || (res == LLONG_MAX / 10 && (str[i]
					- '0') > LLONG_MAX % 10))
			return (-1);
		res = (res * 10) + (str[i] - '0');
		i++;
	}
	return (res * sign);
}

/*
 * File operations - read-only
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
 * File operations - write, create, truncate
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
 * File operations - append mode
 */
int	open_file_append(const char *file_name)
{
	int	fd;

	fd = open(file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		return (-1);
	return (fd);
}
