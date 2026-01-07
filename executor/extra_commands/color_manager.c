/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_manager.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 19:23:33 by akivam            #+#    #+#             */
/*   Updated: 2025/12/22 19:24:17 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifdef BONUS

# include "extra_commands.h"
# include "minishell.h"
# include "utils.h"

static char	*get_foreground_color(char *name)
{
	if (ft_strcmp(name, "red") == 0)
		return (C_RED);
	if (ft_strcmp(name, "green") == 0)
		return (C_GREEN);
	if (ft_strcmp(name, "yellow") == 0)
		return (C_YELLOW);
	if (ft_strcmp(name, "blue") == 0)
		return (C_BLUE);
	if (ft_strcmp(name, "magenta") == 0)
		return (C_MAGENTA);
	if (ft_strcmp(name, "cyan") == 0)
		return (C_CYAN);
	if (ft_strcmp(name, "white") == 0)
		return (C_WHITE);
	return (NULL);
}

static char	*get_background_color(char *name)
{
	if (ft_strcmp(name, "red") == 0)
		return (BG_RED);
	if (ft_strcmp(name, "green") == 0)
		return (BG_GREEN);
	if (ft_strcmp(name, "yellow") == 0)
		return (BG_YELLOW);
	if (ft_strcmp(name, "blue") == 0)
		return (BG_BLUE);
	if (ft_strcmp(name, "magenta") == 0)
		return (BG_MAGENTA);
	if (ft_strcmp(name, "cyan") == 0)
		return (BG_CYAN);
	if (ft_strcmp(name, "black") == 0)
		return (BG_BLACK);
	return (NULL);
}

static char	*get_ansi_code(char *name, int is_background)
{
	if (!name)
		return (NULL);
	if (ft_strcmp(name, "reset") == 0)
		return (C_RESET);
	if (is_background)
		return (get_background_color(name));
	else
		return (get_foreground_color(name));
}

void	set_name_color(char **args, t_shell *shell)
{
	char	*code;

	if (!args[1])
	{
		shell->exit_status = 1;
		return ;
	}
	code = get_ansi_code(args[1], 0);
	if (code)
		shell->terminal_name_color = gc_strdup(shell->global_arena, code);
	shell->exit_status = 0;
}

void	set_name_background_color(char **args, t_shell *shell)
{
	char	*code;

	if (!args[1])
	{
		shell->exit_status = 1;
		return ;
	}
	code = get_ansi_code(args[1], 1);
	if (code)
		shell->terminal_name_bg_color = gc_strdup(shell->global_arena, code);
	shell->exit_status = 0;
}

#endif
