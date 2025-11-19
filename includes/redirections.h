#ifndef REDIRECTIONS_H
# define REDIRECTIONS_H

# include "minishell.h"

/* ========== REDIRECTIONS ========== */
int		setup_redirections(t_redir *redirs, t_shell *shell);
void	handle_heredoc(t_redir *redir, t_shell *shell);
void	restore_fds(t_shell *shell);
void	close_all_fds(void);

#endif