/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 19:20:48 by akivam            #+#    #+#             */
/*   Updated: 2025/12/22 16:05:04 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "minishell.h"
# include <termios.h>

void	executor_run(t_shell *shell);
int		execute_ast(t_ast_node *ast_node, t_shell *shell);

void	execute_command(t_cmd *cmd, t_shell *shell);
void	execute_pipe(t_ast_node *left, t_ast_node *right, t_shell *shell);
int		setup_redirections(t_redir *redirs, t_shell *shell);

char	*expand_heredoc_line(char *line, t_shell *shell);
int		process_heredoc_line(char *line, int fd, t_redir *redir,
			t_shell *shell);
void	setup_raw_mode(struct termios *old_term);
int		process_char(char c, char *buffer, int *pos, int *len);
char	*read_line_raw(char *prompt);
void	read_heredoc_loop(int fd, t_redir *redir, t_shell *shell);
char	*get_heredoc_filename(int counter, t_shell *shell);
int		process_cmd_heredoc(t_cmd *cmd, t_shell *shell);
int		process_ast_heredocs(t_ast_node *ast, t_shell *shell);
void	clean_heredoc(t_cmd *cmd);
void	clean_ast_heredocs(t_ast_node *ast);

void	expand_cmd_args(t_cmd *cmd, t_shell *shell);

void	validate_and_execute(char *cmd_path, t_cmd *cmd, t_shell *shell);
void	exec_child_process(t_cmd *cmd, t_shell *shell);
void	execute_builtin_with_redir(t_cmd *cmd, t_shell *shell);
void	handle_exit_status(t_shell *shell, int status);
int		prepare_cmd_execution(t_cmd *cmd, t_shell *shell);

int		is_builtin(char *cmd);
int		execute_builtin(char **args, t_shell *shell);

#endif
