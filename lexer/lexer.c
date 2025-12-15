/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 00:00:21 by harici            #+#    #+#             */
/*   Updated: 2025/12/12 00:00:25 by harici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"

static void	process_operator(char *input, int *i, t_token **tokens, t_shell *sh)
{
	t_token_type	op_type;

	op_type = handle_operator(input, i);
	add_token(tokens, create_token(op_type,
			get_operator_value(op_type), sh->cmd_arena));
}

static void	process_word(char *input, int *i, t_token **tokens, t_shell *shell)
{
	char	*word;

	word = extract_word(input, i, shell);
	if (word)
		add_token(tokens, create_token(TOKEN_WORD, word, shell->cmd_arena));
}

t_token	*lexer(char *input, t_shell *shell)
{
	t_token	*tokens;
	int		i;

	if (!input || !shell)
		return (NULL);
	tokens = NULL;
	i = 0;
	while (input[i])
	{
		while (input[i] && is_whitespace(input[i]))
			i++;
		if (!input[i])
			break ;
		if (is_special_char(input[i]))
			process_operator(input, &i, &tokens, shell);
		else
			process_word(input, &i, &tokens, shell);
	}
	return (tokens);
}
