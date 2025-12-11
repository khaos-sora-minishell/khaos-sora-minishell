/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 00:00:46 by harici            #+#    #+#             */
/*   Updated: 2025/12/12 00:00:47 by harici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANDER_H
# define EXPANDER_H

# include "minishell.h"
/* ========== EXPANDER ========== */
void	expand_ast(t_ast_node *root, t_shell *shell);
char	*expand_variables(char *str, t_shell *shell);
char	*expand_wildcard(char *pattern, void *arena);
char	*get_exit_status_str(int status, void *arena);

#endif
