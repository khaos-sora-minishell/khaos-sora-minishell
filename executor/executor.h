/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 19:20:48 by akivam            #+#    #+#             */
/*   Updated: 2025/12/21 22:04:03 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "minishell.h"

void	executor_run(t_shell *shell);
void	execute_ast(t_ast_node *ast, t_shell *shell);
void	execute_command(t_cmd *cmd, t_shell *shell);
void	execute_pipe(t_ast_node *left, t_ast_node *right, t_shell *shell);

int		setup_redirections(t_redir *redirs, t_shell *shell);
int		process_cmd_heredoc(t_cmd *cmd, t_shell *shell);
void	check_file_status(char *cmd_path);
void	expand_cmd_args(t_cmd *cmd, t_shell *shell);

int		is_builtin(char *cmd);
int		execute_builtin(char **args, t_shell *shell);

#endif
