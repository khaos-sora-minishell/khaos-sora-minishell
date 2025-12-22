/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 04:00:17 by harici            #+#    #+#             */
/*   Updated: 2025/12/22 12:07:53 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

static void	process_expansion(char *str, char *result, t_shell *shell,
		t_expand_ctx *ctx)
{
	char	*expanded;

	if (!ctx->quote && (str[ctx->i] == '\'' || str[ctx->i] == '"'))
		ctx->quote = str[ctx->i++];
	else if (ctx->quote && str[ctx->i] == ctx->quote)
	{
		ctx->quote = 0;
		ctx->i++;
	}
	else if (str[ctx->i] == '$' && ctx->quote != '\'')
	{
		expanded = process_dollar(str, &ctx->i, shell);
		ft_memcpy(result + ctx->j, expanded, gc_strlen(expanded));
		ctx->j += gc_strlen(expanded);
	}
	else
		result[ctx->j++] = str[ctx->i++];
}

static void	update_len(char *str, t_expand_ctx *ctx, size_t *len,
		t_shell *shell)
{
	char	*expanded;

	if (!ctx->quote && (str[ctx->i] == '\'' || str[ctx->i] == '"'))
		ctx->quote = str[ctx->i++];
	else if (ctx->quote && str[ctx->i] == ctx->quote)
	{
		ctx->quote = 0;
		ctx->i++;
	}
	else if (str[ctx->i] == '$' && ctx->quote != '\'')
	{
		expanded = process_dollar(str, &ctx->i, shell);
		*len += gc_strlen(expanded);
	}
	else
	{
		(*len)++;
		ctx->i++;
	}
}

static size_t	get_expanded_length(char *str, t_shell *shell)
{
	t_expand_ctx	ctx;
	size_t			len;

	ctx.i = 0;
	ctx.quote = 0;
	len = 0;
	while (str[ctx.i])
		update_len(str, &ctx, &len, shell);
	return (len);
}

char	*expand_string(char *str, t_shell *shell)
{
	char			*result;
	t_expand_ctx	ctx;
	size_t			needed;

	if (!str)
		return (NULL);
	needed = get_expanded_length(str, shell);
	result = gc_malloc(shell->cmd_arena, needed + 1);
	ctx.i = 0;
	ctx.j = 0;
	ctx.quote = 0;
	while (str[ctx.i])
		process_expansion(str, result, shell, &ctx);
	result[ctx.j] = '\0';
	return (result);
}
