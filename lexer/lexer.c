/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 20:30:58 by harici            #+#    #+#             */
/*   Updated: 2026/01/13 20:30:58 by harici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"

static int	check_unclosed_quotes(char *input, t_shell *shell)
{
	char	quote;

	quote = check_unclosed_quotes_char(input);
	if (quote)
	{
		ft_putstr_fd("minishell: unexpected EOF while looking for matching `",
			2);
		ft_putchar_fd(quote, 2);
		ft_putendl_fd("'", 2);
		shell->exit_status = 2;
		return (1);
	}
	return (0);
}

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
	if (check_unclosed_quotes(input, shell))
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
	tokens = expand_alias_if_needed(tokens, shell);
	return (tokens);
}
