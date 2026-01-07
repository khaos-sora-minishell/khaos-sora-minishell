/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_true_false.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 19:42:20 by akivam            #+#    #+#             */
/*   Updated: 2026/01/06 22:10:02 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#ifdef BONUS

int	builtin_true(char **args, t_shell *shell)
{
	(void)args;
	(void)shell;
	return (0);
}

int	builtin_false(char **args, t_shell *shell)
{
	(void)args;
	(void)shell;
	return (1);
}

#endif
