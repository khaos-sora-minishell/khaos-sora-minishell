/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   easteregg.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 19:11:12 by akivam            #+#    #+#             */
/*   Updated: 2025/12/22 13:48:50 by harici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EASTEREGG_H
# define EASTEREGG_H

#ifdef EASTEREGG

# define MATRIX_WIDTH 80
# define MATRIX_HEIGHT 24

# include "minishell.h"

typedef void		(*t_easter_func)(char **args, t_shell *shell);

typedef struct s_easter_egg
{
	char			*name;
	t_easter_func	func;
}					t_easter_egg;

/* ========== EASTER EGGS ========== */
int					is_easter_egg(char *cmd);
void				execute_easter_egg(char **args, t_shell *shell);

void				harici_matrix(char **args, t_shell *shell);
void				akivam_executer(char **args, t_shell *shell);
void				set_terminal_name(char **args, t_shell *shell);

#endif /* EASTEREGG */

#endif
