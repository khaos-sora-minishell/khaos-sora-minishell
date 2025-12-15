#include "minishell.h"

/*
 * String helper functions
 */
int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

int	is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '&'
		|| c == '(' || c == ')');
}

int	is_metachar(char c)
{
	return (is_special_char(c) || c == '\'' || c == '\"');
}
