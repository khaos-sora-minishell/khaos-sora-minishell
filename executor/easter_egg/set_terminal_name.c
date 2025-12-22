/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_terminal_name.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 21:47:22 by akivam            #+#    #+#             */
/*   Updated: 2025/12/21 20:35:32 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifdef EASTEREGG

#include "easteregg.h"
#include "garbage_collector.h"
#include "libft.h"
#include "printf.h"

void	set_terminal_name(char **args, t_shell *shell)
{
	t_gc_context	*contex;

	contex = (t_gc_context *)shell->global_arena;
	if (!args[1])
	{
		ft_putendl_fd("Error: armunan is missing", 2);
		ft_putendl_fd("Usage-example: set-terminal sora", 2);
		return ;
	}
	shell->terminal_name = gc_strdup(contex, args[1]);
}

#endif
