/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 04:00:17 by harici            #+#    #+#             */
/*   Updated: 2025/12/22 12:13:03 by harici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

static void	process_expansion(char *str, char *result, t_shell *shell,
		t_expand_contex *contex)
{
	char	*expanded;

	if (!contex->quote && (str[contex->i] == '\'' || str[contex->i] == '"'))
		contex->quote = str[contex->i++];
	else if (contex->quote && str[contex->i] == contex->quote)
	{
		contex->quote = 0;
		contex->i++;
	}
	else if (str[contex->i] == '$' && contex->quote != '\'')
	{
		expanded = process_dollar(str, &contex->i, shell);
		ft_memcpy(result + contex->j, expanded, gc_strlen(expanded));
		contex->j += gc_strlen(expanded);
	}
	else
		result[contex->j++] = str[contex->i++];
}

static void	update_len(char *str, t_expand_contex *contex, size_t *len,
		t_shell *shell)
{
	char	*expanded;

	if (!contex->quote && (str[contex->i] == '\'' || str[contex->i] == '"'))
		contex->quote = str[contex->i++];
	else if (contex->quote && str[contex->i] == contex->quote)
	{
		contex->quote = 0;
		contex->i++;
	}
	else if (str[contex->i] == '$' && contex->quote != '\'')
	{
		expanded = process_dollar(str, &contex->i, shell);
		*len += gc_strlen(expanded);
	}
	else
	{
		(*len)++;
		contex->i++;
	}
}

static size_t	get_expanded_length(char *str, t_shell *shell)
{
	t_expand_contex	contex;
	size_t			len;

	contex.i = 0;
	contex.quote = 0;
	len = 0;
	while (str[contex.i])
		update_len(str, &contex, &len, shell);
	return (len);
}

char	*expand_string(char *str, t_shell *shell)
{
	char			*result;
	t_expand_contex	contex;
	size_t			needed;

	if (!str)
		return (NULL);
	needed = get_expanded_length(str, shell);
	result = gc_malloc(shell->cmd_arena, needed + 1);
	contex.i = 0;
	contex.j = 0;
	contex.quote = 0;
	while (str[contex.i])
		process_expansion(str, result, shell, &contex);
	result[contex.j] = '\0';
	return (result);
}
