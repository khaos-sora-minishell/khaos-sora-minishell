/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 20:30:17 by akivam            #+#    #+#             */
/*   Updated: 2025/12/07 20:40:17 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

int	ft_strcmp(const char *s1, const char *s2);
int	is_whitespace(char c);
int	is_special_char(char c);

/* File utilities */
int	open_file(const char *file_name);
int	open_file_write(const char *file_name);
int	open_file_append(const char *file_name);

#endif