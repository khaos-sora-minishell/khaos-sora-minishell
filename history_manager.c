/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_manager.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 18:16:43 by akivam            #+#    #+#             */
/*   Updated: 2025/12/20 18:21:52 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <readline/history.h>

void	init_history(t_shell *shell)
{
	(void)shell;
}

void	add_history_entry(t_shell *shell, char *line)
{
	(void)shell;
	if (line && *line)
		add_history(line);
}
