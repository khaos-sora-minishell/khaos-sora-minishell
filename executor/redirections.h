/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 19:20:48 by akivam            #+#    #+#             */
/*   Updated: 2026/02/22 00:03:31 by harici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECTIONS_H
# define REDIRECTIONS_H

# include "shell.h"

/* ========== REDIRECTIONS ========== */
int		setup_redirections(t_redir *redirs, t_shell *shell);
void	handle_heredoc(t_redir *redir, t_shell *shell);
void	restore_fds(t_shell *shell);
void	close_all_fds(void);

#endif
