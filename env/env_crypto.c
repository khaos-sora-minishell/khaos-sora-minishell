/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_crypto.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 22:49:15 by akivam            #+#    #+#             */
/*   Updated: 2025/12/21 19:43:59 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

unsigned long	fnv1a_hash(char *str)
{
	unsigned long	hash;
	int				i;

	hash = FNV_OFFSET;
	i = 0;
	while (str[i])
	{
		hash = hash ^ (unsigned char)str[i];
		hash = hash * FNV_PRIME_64;
		i++;
	}
	return (hash % ENV_TABLE_SIZE);
}

void	xor_cipher(char *str)
{
	(void)str;
	return ;
	// DISABLED FOR TESTING - Original code:
	// int	i;
	// if (!str)
	// 	return ;
	// i = 0;
	// while (str[i])
	// {
	// 	str[i] = str[i] ^ XOR_KEY;
	// 	i++;
	// }
}
