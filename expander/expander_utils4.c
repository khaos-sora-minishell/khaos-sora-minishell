/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils4_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 00:32:05 by harici            #+#    #+#             */
/*   Updated: 2026/01/18 00:32:05 by harici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

static void	handle_dollar_exp(char *str, char *result, t_shell *shell,
		t_expand_contex *contex)
{
	char	*expanded;

	expanded = process_dollar(str, &contex->i, shell);
	ft_memcpy(result + contex->j, expanded, gc_strlen(expanded));
	contex->j += gc_strlen(expanded);
}

void	process_expansion(char *str, char *result, t_shell *shell,
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
	else if (str[contex->i] == '~' && !contex->quote && contex->i == 0)
	{
		expanded = process_tilde(str, &contex->i, shell);
		ft_memcpy(result + contex->j, expanded, gc_strlen(expanded));
		contex->j += gc_strlen(expanded);
	}
	else if (str[contex->i] == '$' && contex->quote != '\'')
		handle_dollar_exp(str, result, shell, contex);
	else
		result[contex->j++] = str[contex->i++];
}

void	update_len(char *str, t_expand_contex *contex, size_t *len,
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
	else if (str[contex->i] == '~' && !contex->quote && contex->i == 0)
	{
		expanded = process_tilde(str, &contex->i, shell);
		*len += gc_strlen(expanded);
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
