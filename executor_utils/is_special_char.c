/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_special_char.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 20:30:30 by akivam            #+#    #+#             */
/*   Updated: 2025/12/07 20:30:31 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Check if character is special shell character
 */
int	is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '&' || c == '('
		|| c == ')');
}
