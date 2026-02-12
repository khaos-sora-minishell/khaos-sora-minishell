/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 20:29:04 by harici            #+#    #+#             */
/*   Updated: 2026/01/13 20:29:04 by harici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "shell.h"

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
	result = gc_calloc(shell->cmd_arena, needed + 1, 1);
	contex.i = 0;
	contex.j = 0;
	contex.quote = 0;
	while (str[contex.i])
		process_expansion(str, result, shell, &contex);
	result[contex.j] = '\0';
	return (result);
}
