/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_manager_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 18:16:43 by akivam            #+#    #+#             */
/*   Updated: 2026/01/18 01:45:07 by akivam           ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <fcntl.h>
#include <readline/history.h>

void	init_history(t_shell *shell)
{
	shell->history_file = get_history_path(shell);
	if (shell->history_file)
	{
		load_history_from_file(shell->history_file);
		shell->history_fd = open(shell->history_file,
				O_WRONLY | O_CREAT | O_APPEND, 0644);
	}
}

void	add_history_entry(t_shell *shell, char *line)
{
	if (!line || !*line)
		return ;
	if (get_signal() == SIGINT)
		return ;
	add_history(line);
	if (shell->history_fd > 0)
		ft_putendl_fd(line, shell->history_fd);
}
