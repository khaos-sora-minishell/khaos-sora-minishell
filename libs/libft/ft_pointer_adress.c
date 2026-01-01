/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pointer_adress.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 18:43:19 by akivam            #+#    #+#             */
/*   Updated: 2025/10/16 18:44:31 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_pointer_adress(size_t number, int *len, char *base)
{
	if (number >= 16)
		ft_pointer_adress(number / 16, len, base);
	ft_putchar(base[number % 16], len);
}

void	ft_print_pointer_address(void *ptr, int *len)
{
	if (ptr == NULL)
		return (ft_putstr("(nil)", len));
	ft_putstr("0x", len);
	ft_pointer_adress((size_t)ptr, len, HEX_L);
}
