/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoll.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/04 21:17:59 by akivam            #+#    #+#             */
/*   Updated: 2026/01/04 21:18:11 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

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
