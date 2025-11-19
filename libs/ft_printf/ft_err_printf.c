/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_err_printf.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 18:43:19 by akivam            #+#    #+#             */
/*   Updated: 2025/10/16 18:43:33 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "printf.h"

static void	handle_format(const char *format, va_list args, int i, int *len)
{
	char	flag;

	flag = format[i + 1];
	if (flag == 'd' || flag == 'i')
		ft_err_putnbr(va_arg(args, int), len);
	else if (flag == 'u')
		ft_err_putnbr_unsigned(va_arg(args, unsigned int), len);
	else if (flag == 'c')
		ft_err_putchar(va_arg(args, int), len);
	else if (flag == 's')
		ft_err_putstr(va_arg(args, char *), len);
	else if (flag == '%')
		ft_err_putchar('%', len);
	else if (flag == 'p')
		ft_err_print_pointer_address(va_arg(args, void *), len);
	else if (flag == 'x')
		ft_err_puthex(va_arg(args, unsigned int), len, HEX_L);
	else if (flag == 'X')
		ft_err_puthex(va_arg(args, unsigned int), len, HEX_B);
	else
		ft_err_putchar(flag, len);
}

int	ft_err_printf(const char *format, ...)
{
	va_list	args;
	int		i;
	int		len;

	i = 0;
	len = 0;
	va_start(args, format);
	while (format[i])
	{
		if (format[i] == '%')
		{
			if (!format[i + 1])
			{
				va_end(args);
				return (-1);
			}
			handle_format(format, args, i, &len);
			i += 2;
			continue ;
		}
		ft_err_putchar(format[i++], &len);
	}
	va_end(args);
	return (len);
}
