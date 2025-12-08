/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   easter_egg.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 20:52:24 by akivam            #+#    #+#             */
/*   Updated: 2025/12/08 21:18:11 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "easteregg.h"
#include "utils.h"

/*function pointer*/
static t_easter_egg	*get_easter_egg(void)
{
	static t_easter_egg	easter[] = {
	{"harici_pars", harici_pars},
	{"akivam_executer", akivam_executer},
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

/*exec easter func*/

void	execute_easter_egg(char *cmd, t_shell *shell)
{
	t_easter_egg	*easters;
	int				i;

	if (!cmd)
		return ;
	easters = get_easter_egg();
	i = 0;
	while (easters[i].name)
	{
		if (ft_strcmp(cmd, easters[i].name) == 0)
		{
			easters[i].func(shell);
			return ;
		}
		i++;
	}
}
