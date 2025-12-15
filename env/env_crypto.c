/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_crypto.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 22:49:15 by akivam            #+#    #+#             */
/*   Updated: 2025/12/15 13:33:51 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** FNV-1a Hash Algoritması
** String'i alır, benzersiz bir 'unsigned long' üretir.
** Bu sayı, tablodaki index'imiz olacak.
*/

unsigned long fnv1a_hash(char *str)
{
	unsigned long hash;
	int i;
	hash = FNV_OFFSET;
	i=0;
	while (str[i])
	{
		hash = hash ^ (unsigned char)str[i]; //XOR
		hash = hash * FNV_PRIME_64; // Asal sayı ile çarp
		i++;
	}
	return (hash % ENV_TABLE_SIZE); // Tablo boyutuna göre mod al
}

/*
** XOR Toggle (Switch)
** Veriyi şifreler VEYA şifresini çözer.
** Memory dostu: Yerinde (in-place) değişiklik yapar.
*/

void xor_cipher(char *str)
{
	int i;
	if(!str)
		return ;
	i = 0;
	while(str[i])
	{
		str[i] = str[i] ^ XOR_KEY;
		i++;
	}
}

