/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_vs_executer_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 20:59:45 by akivam            #+#    #+#             */
/*   Updated: 2026/01/20 21:32:48 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "extra_commands.h"
#include "libft.h"
#include "shell.h"

void	akivam_executor(char **args, t_shell *shell)
{
	(void)args;
	ft_putendl_fd("Please write it without going overboard 🥺\n", 1);
	shell->exit_status = 0;
}

void	harici_parser(char **args, t_shell *shell)
{
	(void)args;
	ft_putendl_fd("Please write parse 🥺🥺🥺🥺🥺\n", 1);
	shell->exit_status = 0;
}
