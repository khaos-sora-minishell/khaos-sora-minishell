/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 21:14:50 by harici            #+#    #+#             */
/*   Updated: 2026/01/10 18:50:10 by harici           ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"
#include "parser.h"

static int	heredoc_has_outer_quotes(char *str, char *quote_char)
{
	int	len;

	if (!str || !*str)
		return (0);
	len = ft_strlen(str);
	if (len >= 2 && ((str[0] == '\'' && str[len - 1] == '\'')
			|| (str[0] == '\"' && str[len - 1] == '\"')))
	{
		*quote_char = str[0];
		return (1);
	}
	return (0);
}

static char	*heredoc_strip_quotes(char *str, void *arena)
{
	int		len;
	char	*result;
	int		i;

	len = ft_strlen(str);
	if (len < 2)
		return (gc_strdup(arena, str));
	result = gc_malloc(arena, len - 1);
	if (!result)
		return (NULL);
	i = 0;
	while (i < len - 2)
	{
		result[i] = str[i + 1];
		i++;
	}
	result[i] = '\0';
	return (result);
}

char	*parse_heredoc_delimiter(char *raw_delim, int *should_expand,
		void *arena)
{
	char	quote_char;

	if (heredoc_has_outer_quotes(raw_delim, &quote_char))
	{
		*should_expand = 0;
		return (heredoc_strip_quotes(raw_delim, arena));
	}
	*should_expand = 1;
	return (gc_strdup(arena, raw_delim));
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
		redir->delimiter = parse_heredoc_delimiter(file, &redir->should_expand,
				arena);
		redir->file = NULL;
	}
	else
	{
		redir->file = gc_strdup(arena, file);
		redir->delimiter = NULL;
		redir->should_expand = 0;
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
