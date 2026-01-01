/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_error2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/01 21:48:45 by akivam            #+#    #+#             */
/*   Updated: 2026/01/01 21:48:56 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor_error.h"
#include "libft.h"
#include "minishell.h"
#include <stdlib.h>
#include <unistd.h>

void	handle_no_such_file(char *cmd, t_shell *shell)
{
	exit_with_error(cmd, ERR_NO_FILE, EXIT_CMD_NOT_FOUND, shell);
}
