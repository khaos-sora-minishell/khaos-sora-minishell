/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 21:14:56 by harici            #+#    #+#             */
/*   Updated: 2026/01/10 18:50:09 by harici           ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"
#include "parser.h"

int	is_redirection_token(t_token_type type)
{
	return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_REDIR_APPEND || type == TOKEN_HEREDOC);
}

#ifdef BONUS

int	is_operator_token(t_token_type type)
{
	return (type == TOKEN_PIPE || type == TOKEN_AND || type == TOKEN_OR
		|| type == TOKEN_LPAREN || type == TOKEN_RPAREN);
}

#else

int	is_operator_token(t_token_type type)
{
	return (type == TOKEN_PIPE);
}

#endif

t_cmd	*create_cmd(void *arena)
{
	t_cmd	*cmd;

	cmd = gc_malloc(arena, sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->redirs = NULL;
	return (cmd);
}
