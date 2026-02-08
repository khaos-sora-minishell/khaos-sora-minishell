/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra_commands.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 20:50:44 by akivam            #+#    #+#             */
/*   Updated: 2026/02/09 02:16:59 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXTRA_COMMANDS_H
# define EXTRA_COMMANDS_H

# include "minishell.h"
# include "tetris.h"
# include <termios.h>

# define C_RESET "\033[0m"
# define C_RED "\033[31m"
# define C_GREEN "\033[32m"
# define C_YELLOW "\033[33m"
# define C_BLUE "\033[34m"
# define C_MAGENTA "\033[35m"
# define C_CYAN "\033[36m"
# define C_WHITE "\033[37m"

# define BG_RED "\033[41m"
# define BG_GREEN "\033[42m"
# define BG_YELLOW "\033[43m"
# define BG_BLUE "\033[44m"
# define BG_MAGENTA "\033[45m"
# define BG_CYAN "\033[46m"
# define BG_BLACK "\033[40m"

# include "minishell.h"
# include "tetris.h"

typedef void		(*t_extra_command_func)(char **args, t_shell *shell);

typedef struct s_extra_command
{
	char					*name;
	t_extra_command_func	func;
}	t_extra_command;

int					is_extra_command(char *cmd);
void				execute_extra_command(char **args, t_shell *shell);
void				akivam_executor(char **args, t_shell *shell);
void				harici_parser(char **args, t_shell *shell);
void				set_terminal_name(char **args, t_shell *shell);

void				set_prompt(char **args, t_shell *shell);
void				set_background(char **args, t_shell *shell);
void				update_ps1(t_shell *shell);
char				*get_prompt_color(char *arg);
char				*get_background_color(char *arg);
char				*get_color_code(char *arg, int is_background);

#endif
