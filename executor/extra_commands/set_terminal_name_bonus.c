/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_terminal_name_bonus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 21:47:22 by akivam            #+#    #+#             */
/*   Updated: 2026/01/13 01:34:42 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifdef BONUS

# include "extra_commands_bonus.h"
# include "garbage_collector.h"
# include "libft.h"
# include "minishell.h"

void	set_terminal_name(char **args, t_shell *shell)
{
	t_gc_context	*contex;

	contex = (t_gc_context *)shell->global_arena;
	if (!args[1])
	{
		ft_err_printf("Error: armunan is missing");
		ft_err_printf("Usage-example: set-terminal sora");
		return ;
	}
	shell->terminal_name = gc_strdup(contex, args[1]);
}

#endif
