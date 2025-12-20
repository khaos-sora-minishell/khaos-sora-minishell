/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_tty.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 20:55:31 by akivam            #+#    #+#             */
/*   Updated: 2025/12/09 20:55:57 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "printf.h"

int	builtin_tty(char **args, t_shell *shell)
{
	char	*tty;

	(void)args;
	(void)shell;
	if (isatty(0))
	{
		tty = ttyname(0);
		if (tty)
		{
			printf("%s\n", tty);
			return (0);
		}
	}
	printf("not a tty\n");
	return (1);
}
