#include "minishell.h"

/*
 * create_token - Creates a new token with given type and value
 * @type: Token type (WORD, PIPE, etc.)
 * @value: String value of the token
 * @arena: GC arena for memory allocation
 *
 * Return: Pointer to newly created token
 */
t_token	*create_token(t_token_type type, char *value, void *arena)
{
	t_token	*token;

	token = gc_malloc(arena, sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = gc_strdup(arena, value);
	token->next = NULL;
	return (token);
}

/*
 * add_token - Adds a token to the end of token list
 * @head: Pointer to the head of token list
 * @new_token: Token to be added
 *
 * Appends new_token to the end of the linked list
 */
void	add_token(t_token **head, t_token *new_token)
{
	t_token	*current;

	if (!*head)
	{
		*head = new_token;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_token;
}

/*
 * extract_word - Extracts a word from input string
 * @input: Input string
 * @i: Current position pointer
 * @shell: Shell structure containing GC arena
 *
 * Reads characters until whitespace or special character is found
 * Return: Extracted word as a string
 */
static char	*extract_word(char *input, int *i, t_shell *shell)
{
	int		start;
	int		len;
	char	*word;
	int		j;

	start = *i;
	while (input[*i] && !is_whitespace(input[*i])
		&& !is_special_char(input[*i]))
		(*i)++;
	len = *i - start;
	word = gc_malloc(shell->cmd_arena, len + 1);
	if (!word)
		return (NULL);
	j = 0;
	while (j < len)
	{
		word[j] = input[start + j];
		j++;
	}
	word[j] = '\0';
	return (word);
}

/*
 * lexer - Tokenizes input string into linked list of tokens
 * @input: Raw string from readline
 * @shell: Shell structure containing GC arena
 *
 * Basic tokenization: splits input by whitespace into WORD tokens
 * Return: Head of token linked list, NULL on error
 */
t_token	*lexer(char *input, t_shell *shell)
{
	t_token	*tokens;
	char	*word;
	int		i;

	if (!input || !shell)
		return (NULL);
	tokens = NULL;
	i = 0;
	while (input[i])
	{
		// Skip whitespace
		while (input[i] && is_whitespace(input[i]))
			i++;
		if (!input[i])
			break ;
		// Extract word and create token
		if (!is_special_char(input[i]))
		{
			word = extract_word(input, &i, shell);
			if (word)
				add_token(&tokens, create_token(TOKEN_WORD, word,
					shell->cmd_arena));
		}
		else
			i++; // Skip special chars for now (TODO: handle operators)
	}
	return (tokens);
}
