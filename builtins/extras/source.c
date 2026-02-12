/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   source.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 21:27:02 by akivam            #+#    #+#             */
/*   Updated: 2026/02/09 02:19:24 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "utils.h"
#include "executor/executor.h"

static int	validate_and_open_source(char **args)
{
	int	fd;

	if (!args[1])
	{
		ft_err_printf(".: filename argument required\n");
		return (-2);
	}
	fd = open_file(args[1]);
	if (fd == -1)
	{
		ft_err_printf(".: %s: No such file or directory\n", args[1]);
		return (-1);
	}
	return (fd);
}

int	builtin_source(char **args, t_shell *shell)
{
	int			fd;
	char		*line;
	char		*old_input;
	t_token		*tokens;
	t_ast_node	*ast;

	fd = validate_and_open_source(args);
	if (fd == -2)
		return (2);
	if (fd == -1)
		return (1);
	line = get_next_line(fd);
	while (line != NULL)
	{
		old_input = shell->current_input;
		shell->current_input = line;
		tokens = lexer(line, shell);
		ast = parser(tokens, shell);
		if (ast)
			execute_ast(ast, shell);
		shell->current_input = old_input;
		free(line);
	}
	close(fd);
	return (shell->exit_status);
}
