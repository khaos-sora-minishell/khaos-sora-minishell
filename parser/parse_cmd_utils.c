/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 21:24:26 by akivam            #+#    #+#             */
/*   Updated: 2025/12/21 21:28:27 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

t_redir	*create_redir(t_token_type type, char *file, void *arena)
{
	t_redir	*redir;

	redir = gc_malloc(arena, sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = type;
	if (type == TOKEN_HEREDOC)
	{
		redir->delimiter = gc_strdup(arena, file);
		redir->file = NULL;
	}
	else
	{
		redir->file = gc_strdup(arena, file);
		redir->delimiter = NULL;
	}
	redir->heredoc_tmpfile = NULL;
	redir->next = NULL;
	return (redir);
}

void	add_redir(t_redir **list, t_redir *new_redir)
{
	t_redir	*current;

	if (!new_redir)
		return ;
	if (!*list)
	{
		*list = new_redir;
		return ;
	}
	current = *list;
	while (current->next)
		current = current->next;
	current->next = new_redir;
}
