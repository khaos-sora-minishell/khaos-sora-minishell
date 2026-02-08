/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_utils_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 20:50:24 by akivam            #+#    #+#             */
/*   Updated: 2026/02/07 20:50:28 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "extra_commands.h"
#include "libft.h"
#include "utils.h"

char	*get_prompt_color(char *arg)
{
	if (ft_strcmp(arg, "red") == 0)
		return (C_RED);
	if (ft_strcmp(arg, "green") == 0)
		return (C_GREEN);
	if (ft_strcmp(arg, "yellow") == 0)
		return (C_YELLOW);
	if (ft_strcmp(arg, "blue") == 0)
		return (C_BLUE);
	if (ft_strcmp(arg, "magenta") == 0)
		return (C_MAGENTA);
	if (ft_strcmp(arg, "cyan") == 0)
		return (C_CYAN);
	if (ft_strcmp(arg, "white") == 0)
		return (C_WHITE);
	return (NULL);
}

char	*get_background_color(char *arg)
{
	if (ft_strcmp(arg, "red") == 0)
		return (BG_RED);
	if (ft_strcmp(arg, "green") == 0)
		return (BG_GREEN);
	if (ft_strcmp(arg, "yellow") == 0)
		return (BG_YELLOW);
	if (ft_strcmp(arg, "blue") == 0)
		return (BG_BLUE);
	if (ft_strcmp(arg, "magenta") == 0)
		return (BG_MAGENTA);
	if (ft_strcmp(arg, "cyan") == 0)
		return (BG_CYAN);
	if (ft_strcmp(arg, "black") == 0)
		return (BG_BLACK);
	return (NULL);
}

char	*get_color_code(char *arg, int is_background)
{
	if (!arg)
		return (NULL);
	if (ft_strcmp(arg, "reset") == 0)
		return (C_RESET);
	if (is_background)
		return (get_background_color(arg));
	else
		return (get_prompt_color(arg));
}
