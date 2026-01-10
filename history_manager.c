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
#include <fcntl.h>
#include <readline/history.h>


#ifdef BONUS

void init_history(t_shell *shell) {
  shell->history_file = get_history_path(shell);
  if (shell->history_file) {
    load_history_from_file(shell->history_file);
    shell->history_fd =
        open(shell->history_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
  }
}

void add_history_entry(t_shell *shell, char *line) {
  if (!line || !*line)
    return;
  if (get_signal() == SIGINT)
    return;
  add_history(line);
  if (shell->history_fd > 0) {
    ft_putendl_fd(line, shell->history_fd);
  }
}

#else

void init_history(t_shell *shell) { (void)shell; }

void add_history_entry(t_shell *shell, char *line) {
  (void)shell;
  if (line && *line)
    add_history(line);
}

#endif
