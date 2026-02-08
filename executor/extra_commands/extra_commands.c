/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra_commands_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 21:01:11 by akivam            #+#    #+#             */
/*   Updated: 2026/02/02 18:27:10 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "extra_commands.h"
#include "utils.h"

static t_extra_command	*get_extra_command(void)
{
	static t_extra_command	extra_cmd[] = {
	{"akivam-executor", akivam_executor},
	{"harici-parser", harici_parser},
	{"set-terminal", set_terminal_name},
	{"set-prompt-color", set_prompt},
	{"set-prompt-background", set_background},
	{"tetris-42", start_game},
	{NULL, NULL}};

	return (extra_cmd);
}

int	is_extra_command(char *cmd)
{
	t_extra_command	*extra_cmd;
	int				i;

	if (!cmd)
		return (0);
	extra_cmd = get_extra_command();
	i = 0;
	while (extra_cmd[i].name)
	{
		if (ft_strcmp(cmd, extra_cmd[i].name) == 0)
			return (1);
		i++;
	}
	return (0);
}

void	execute_extra_command(char **args, t_shell *shell)
{
	t_extra_command	*extra_cmd;
	int				i;

	if (!args || !args[0])
		return ;
	extra_cmd = get_extra_command();
	i = 0;
	while (extra_cmd[i].name)
	{
		if (ft_strcmp(args[0], extra_cmd[i].name) == 0)
		{
			extra_cmd[i].func(args, shell);
			return ;
		}
		i++;
	}
}
