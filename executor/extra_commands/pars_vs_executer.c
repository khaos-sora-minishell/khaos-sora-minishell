/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_vs_executer.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 20:59:45 by akivam            #+#    #+#             */
/*   Updated: 2025/12/22 13:20:47 by harici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifdef BONUS

# include "extra_commands.h"
# include "libft.h"
# include "minishell.h"

void	akivam_executer(char **args, t_shell *shell)
{
	(void)args;
	ft_putendl_fd("Please write it without going overboard 🥺\n", 1);
	shell->exit_status = 0;
}

#endif
