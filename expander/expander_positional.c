/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_positional.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 12:57:52 by akivam            #+#    #+#             */
/*   Updated: 2026/01/11 12:57:52 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

char	*expand_positional_arg(t_shell *shell, int index)
{
	(void)index;
	return (gc_strdup(shell->cmd_arena, ""));
}

char	*expand_all_args(t_shell *shell)
{
	return (gc_strdup(shell->cmd_arena, ""));
}
