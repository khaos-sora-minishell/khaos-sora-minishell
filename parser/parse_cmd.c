#include "minishell.h"
#include "parser.h"

/*
 * Token'ın redirection olup olmadığını kontrol eder
 */
int	is_redirection_token(t_token_type type)
{
	return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_REDIR_APPEND || type == TOKEN_HEREDOC);
}

/*
 * Token'ın operator olup olmadığını kontrol eder (pipe, &&, ||)
 */
int	is_operator_token(t_token_type type)
{
	return (type == TOKEN_PIPE || type == TOKEN_AND || type == TOKEN_OR);
}

/*
 * Yeni bir t_cmd oluşturur
 */
t_cmd	*create_cmd(void *arena)
{
	t_cmd	*cmd;

	cmd = gc_malloc(arena, sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->redirs = NULL;
	return (cmd);
}

/*
 * Yeni bir t_redir oluşturur
 */
t_redir	*create_redir(t_token_type type, char *file, void *arena)
{
	t_redir	*redir;

	redir = gc_malloc(arena, sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->file = gc_strdup(arena, file);
	redir->delimiter = NULL;
	redir->heredoc_tmpfile = NULL;
	redir->next = NULL;
	return (redir);
}

/*
 * Redirection'ı listeye ekler
 */
void	add_redir(t_redir **list, t_redir *new_redir)
{
	t_redir	*current;

	if (!new_redir)
		return ;
	if (!*list)
	{
		*list = new_redir;
		return ;
	}
	current = *list;
	while (current->next)
		current = current->next;
	current->next = new_redir;
}

/*
 * Komutun args dizisine yeni argüman ekler
 */
void	add_arg_to_cmd(t_cmd *cmd, char *arg, void *arena)
{
	int		count;
	char	**new_args;
	int		i;

	count = 0;
	while (cmd->args && cmd->args[count])
		count++;
	new_args = gc_malloc(arena, sizeof(char *) * (count + 2));
	if (!new_args)
		return ;
	i = 0;
	while (i < count)
	{
		new_args[i] = cmd->args[i];
		i++;
	}
	new_args[i] = gc_strdup(arena, arg);
	new_args[i + 1] = NULL;
	cmd->args = new_args;
}

/*
 * Redirection parse eder ve token pointer'ını ilerletir
 * Örnek: < file.txt veya > output.txt
 */
static int	parse_redirection(t_token **tokens, t_cmd *cmd, t_shell *shell)
{
	t_token_type	redir_type;
	t_redir			*redir;

	if (!*tokens || !is_redirection_token((*tokens)->type))
		return (0);
	redir_type = (*tokens)->type;
	*tokens = (*tokens)->next;
	if (!*tokens || (*tokens)->type != TOKEN_WORD)
	{
		syntax_error("newline");
		return (-1);
	}
	redir = create_redir(redir_type, (*tokens)->value, shell->cmd_arena);
	if (!redir)
		return (-1);
	add_redir(&cmd->redirs, redir);
	*tokens = (*tokens)->next;
	return (1);
}

/*
 * Basit komut parse - Token'lardan t_cmd oluşturur
 * Args ve redirectionları ayıklar
 *
 * Örnek: ls -la > file.txt
 * Token'lar: [WORD:ls] [WORD:-la] [REDIR_OUT:>] [WORD:file.txt]
 * Çıktı: t_cmd {args: ["ls", "-la", NULL], redirs: [OUT:file.txt]}
 */
t_cmd	*parse_simple_command(t_token **tokens, t_shell *shell)
{
	t_cmd	*cmd;
	int		ret;

	if (!tokens || !*tokens)
		return (NULL);
	cmd = create_cmd(shell->cmd_arena);
	if (!cmd)
		return (NULL);
	while (*tokens)
	{
		if (is_operator_token((*tokens)->type))
			break ;
		if (is_redirection_token((*tokens)->type))
		{
			ret = parse_redirection(tokens, cmd, shell);
			if (ret <= 0)
				return (NULL);
		}
		else if ((*tokens)->type == TOKEN_WORD)
		{
			add_arg_to_cmd(cmd, (*tokens)->value, shell->cmd_arena);
			*tokens = (*tokens)->next;
		}
		else
			*tokens = (*tokens)->next;
	}
	return (cmd);
}
