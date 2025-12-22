/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   easter_egg.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 20:52:24 by akivam            #+#    #+#             */
/*   Updated: 2025/12/22 14:32:06 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifdef EASTEREGG

#include "easteregg.h"
#include "utils.h"

static t_easter_egg	*get_easter_egg(void)
{
	static t_easter_egg	easter[] = {
	{"harici_matrix", harici_matrix},
	{"akivam_executer", akivam_executer},
	{"set-terminal", set_terminal_name},
	{NULL, NULL}};

	return (easter);
}

int	is_easter_egg(char *cmd)
{
	t_easter_egg	*easters;
	int				i;

	if (!cmd)
		return (0);
	easters = get_easter_egg();
	i = 0;
	while (easters[i].name)
	{
		if (ft_strcmp(cmd, easters[i].name) == 0)
			return (1);
		i++;
	}
	return (0);
}

void	execute_easter_egg(char **args, t_shell *shell)
{
	t_easter_egg	*easters;
	int				i;

	if (!args || !args[0])
		return ;
	easters = get_easter_egg();
	i = 0;
	while (easters[i].name)
	{
		if (ft_strcmp(args[0], easters[i].name) == 0)
		{
			easters[i].func(args, shell);
			return ;
		}
		i++;
	}
}

#endif
