/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_vs_executer_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 20:59:45 by akivam            #+#    #+#             */
/*   Updated: 2026/01/13 01:34:38 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifdef BONUS

# include "extra_commands_bonus.h"
# include "libft.h"
# include "minishell.h"

void	akivam_executer(char **args, t_shell *shell)
{
	(void)args;
	ft_putendl_fd("Please write it without going overboard 🥺\n", 1);
	shell->exit_status = 0;
}

#endif
