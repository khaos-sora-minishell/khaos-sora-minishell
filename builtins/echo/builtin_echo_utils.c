/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:11:07 by akivam            #+#    #+#             */
/*   Updated: 2025/12/21 18:52:15 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "garbage_collector.h"
#include "libft.h"
#include "minishell.h"

void	op_upper(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		str[i] = ft_toupper(str[i]);
		i++;
	}
}

void	op_reverse(char *str)
{
	int		len;
	int		i;
	char	temp;

	if (!str)
		return ;
	len = ft_strlen(str);
	i = 0;
	while (i < len / 2)
	{
		temp = str[i];
		str[i] = str[len - 1 - i];
		str[len - 1 - i] = temp;
		i++;
	}
}

void	print_with_escape(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] == '\\' && str[i + 1])
		{
			if (str[i + 1] == 'n')
				write(1, "\n", 1);
			else if (str[i + 1] == 't')
				write(1, "\t", 1);
			else if (str[i + 1] == '\\')
				write(1, "\\", 1);
			else
			{
				write(1, &str[i], 1);
				write(1, &str[i + 1], 1);
			}
			i++;
		}
		else
			write(1, &str[i], 1);
	}
}
