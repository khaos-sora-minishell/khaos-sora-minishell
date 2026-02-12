/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 21:59:14 by harici            #+#    #+#             */
/*   Updated: 2026/01/20 21:59:14 by harici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "parser.h"

void	add_arg_to_cmd(t_cmd *cmd, char *arg, void *arena)
{
	int		count;
	char	**new_args;
	int		i;

	count = 0;
	while (cmd->args && cmd->args[count])
		count++;
	new_args = gc_malloc(arena, sizeof(char *) * (count + 2));
	if (!new_args)
		return ;
	i = 0;
	while (i < count)
	{
		new_args[i] = cmd->args[i];
		i++;
	}
	new_args[i] = gc_strdup(arena, arg);
	new_args[i + 1] = NULL;
	cmd->args = new_args;
}

static int	parse_redirection(t_token **tokens, t_cmd *cmd, t_shell *shell)
{
	t_token_type	redir_type;
	t_redir			*redir;

	if (!*tokens || !is_redirection_token((*tokens)->type))
		return (0);
	redir_type = (*tokens)->type;
	*tokens = (*tokens)->next;
	if (!*tokens || (*tokens)->type != TOKEN_WORD)
	{
		syntax_error("newline");
		return (-1);
	}
	redir = create_redir(redir_type, (*tokens)->value, shell->cmd_arena);
	if (!redir)
		return (-1);
	add_redir(&cmd->redirs, redir);
	*tokens = (*tokens)->next;
	return (1);
}

static int	process_current_token(t_token **tokens, t_cmd *cmd, t_shell *shell)
{
	if (is_redirection_token((*tokens)->type))
	{
		if (parse_redirection(tokens, cmd, shell) <= 0)
			return (-1);
	}
	else if ((*tokens)->type == TOKEN_WORD)
	{
		add_arg_to_cmd(cmd, (*tokens)->value, shell->cmd_arena);
		*tokens = (*tokens)->next;
	}
	else
		*tokens = (*tokens)->next;
	return (0);
}

t_cmd	*parse_simple_command(t_token **tokens, t_shell *shell)
{
	t_cmd	*cmd;

	if (!tokens || !*tokens)
		return (NULL);
	cmd = create_cmd(shell->cmd_arena);
	if (!cmd)
		return (NULL);
	while (*tokens)
	{
		if (is_operator_token((*tokens)->type))
			break ;
		if (process_current_token(tokens, cmd, shell) == -1)
			return (NULL);
	}
	return (cmd);
}
