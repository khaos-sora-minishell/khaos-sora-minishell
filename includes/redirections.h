/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 19:20:48 by akivam            #+#    #+#             */
/*   Updated: 2025/12/07 20:33:38 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECTIONS_H
# define REDIRECTIONS_H

# include "minishell.h"

/* ========== REDIRECTIONS ========== */
int		setup_redirections(t_redir *redirs, t_shell *shell);
void	handle_heredoc(t_redir *redir, t_shell *shell);
void	restore_fds(t_shell *shell);
void	close_all_fds(void);

#endif
