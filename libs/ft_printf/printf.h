/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 18:43:19 by akivam            #+#    #+#             */
/*   Updated: 2025/10/16 18:45:03 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRINTF_H
# define PRINTF_H

# include <stdarg.h>
# include <stdlib.h>
# include <unistd.h>

# define HEX_B "0123456789ABCDEF"
# define HEX_L "0123456789abcdef"

int		ft_printf(const char *format, ...);
void	ft_putchar(char c, int *len);
void	ft_putnbr(int n, int *len);
void	ft_putnbr_unsigned(unsigned int n, int *len);
void	ft_putstr(char *s, int *len);
void	ft_print_pointer_address(void *ptr, int *len);
void	ft_puthex(unsigned int number, int *len, char *base);
int		ft_err_printf(const char *format, ...);
void	ft_err_putchar(char c, int *len);
void	ft_err_putnbr(int n, int *len);
void	ft_err_putnbr_unsigned(unsigned int n, int *len);
void	ft_err_putstr(char *s, int *len);
void	ft_err_print_pointer_address(void *ptr, int *len);
void	ft_err_puthex(unsigned int number, int *len, char *base);

#endif
