/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_error.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 21:42:24 by akivam            #+#    #+#             */
/*   Updated: 2025/12/21 21:17:49 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_ERROR_H
# define EXECUTOR_ERROR_H

# define ERR_CMD_NOT_FOUND "command not found"
# define ERR_NO_FILE "No such file or directory"
# define ERR_PERM_DENIED "Permission denied"
# define ERR_IS_DIR "Is a directory"

# define EXIT_CMD_NOT_FOUND 127
# define EXIT_CANNOT_EXEC 126
# define EXIT_GENERAL 1

void	print_execution_error(char *cmd, char *msg);
void	handle_cmd_not_found(char *cmd);
void	handle_permission_denied(char *cmd);
void	handle_is_directory(char *cmd);

#endif
