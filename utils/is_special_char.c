#include "minishell.h"

/*
 * Check if character is special shell character
 */
int	is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '&' || c == '('
		|| c == ')');
}
