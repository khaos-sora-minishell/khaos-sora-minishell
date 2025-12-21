/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 20:30:17 by akivam            #+#    #+#             */
/*   Updated: 2025/12/21 21:37:43 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# define LLONG_MAX 9223372036854775807

int			ft_strcmp(const char *s1, const char *s2);
int			is_whitespace(char c);
int			is_special_char(char c);

int			open_file(const char *file_name);
int			open_file_write(const char *file_name);
int			open_file_append(const char *file_name);
long long	ft_atoll(const char *str);

#endif
