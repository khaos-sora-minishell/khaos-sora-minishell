/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_vs_executer.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 20:59:45 by akivam            #+#    #+#             */
/*   Updated: 2025/12/10 21:53:37 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "easteregg.h"
#include "libft.h"

void harici_pars(char **args, t_shell *shell)
{
	(void)args;
	ft_putendl_fd("bro write parse plzz 🥺🥺🥺🥺🥺\n",1);
	shell->exit_status = 0;
}

void akivam_executer(char **args, t_shell *shell)
{
	(void)args;
	ft_putendl_fd("Please write it without going overboard 🥺\n",1);
	shell->exit_status = 0;
}