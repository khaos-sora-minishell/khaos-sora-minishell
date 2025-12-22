/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 03:59:58 by harici            #+#    #+#             */
/*   Updated: 2025/12/22 04:31:57 by harici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

char	*extract_var_name(char *str, int *i, t_shell *shell)
{
	int		start;
	int		len;
	char	*name;

	start = *i;
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	len = *i - start;
	if (len == 0)
		return (NULL);
	name = gc_malloc(shell->cmd_arena, len + 1);
	ft_memcpy(name, str + start, len);
	name[len] = '\0';
	return (name);
}

char	*expand_exit_status(t_shell *shell)
{
	return (gc_itoa(shell->cmd_arena, shell->exit_status));
}

char	*expand_variable(char *var_name, t_shell *shell)
{
	char	*value;

	if (!var_name)
		return (gc_strdup(shell->cmd_arena, ""));
	value = env_get(shell->env_table, var_name, shell->cmd_arena);
	if (!value)
		return (gc_strdup(shell->cmd_arena, ""));
	return (value);
}

char	*process_dollar(char *str, int *i, t_shell *shell)
{
	char	*var_name;
	char	*value;

	(*i)++;
	if (str[*i] == '?')
	{
		(*i)++;
		value = expand_exit_status(shell);
		return (value);
	}
	if (!str[*i] || (!ft_isalnum(str[*i]) && str[*i] != '_'))
		return (gc_strdup(shell->cmd_arena, "$"));
	var_name = extract_var_name(str, i, shell);
	value = expand_variable(var_name, shell);
	return (value);
}

void	expand_variables(t_ast_node *ast, t_shell *shell)
{
	t_redir	*redir;

	if (!ast || !shell)
		return ;
	if (ast->type == NODE_CMD && ast->cmd)
	{
		if (ast->cmd->args)
			ast->cmd->args = expand_args(ast->cmd->args, shell);
		redir = ast->cmd->redirs;
		while (redir)
		{
			if (redir->file)
				redir->file = expand_string(redir->file, shell);
			redir = redir->next;
		}
	}
	else if (ast->type == NODE_PIPE || ast->type == NODE_AND
		|| ast->type == NODE_OR)
	{
		expand_variables(ast->left, shell);
		expand_variables(ast->right, shell);
	}
	else if (ast->type == NODE_SUBSHELL)
		expand_variables(ast->subshell_node, shell);
}
