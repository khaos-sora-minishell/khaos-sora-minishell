#include "minishell.h"
#include <term.h>
#include "printf.h"

//tputs fonksiyonu yazdırma işlemi için bir fonksiyon pointer ister

static int ft_outc(int c)
{
	unsigned char ch;

	ch = (unsigned char)c;
	write(1, &ch, 1);
	return 0;
}

int builtin_clear(char **args, t_shell *shell)
{
	char *term_type;
	char *clear_string;
	(void)args;

	term_type = get_env_value(shell->env_list, "TERM");
	if(!term_type)
	{
		printf("minishell: clear: Term not set");
		return 1;
	}
	if(tgetent(NULL, term_type) <= 0)
	{
		printf("minishell: clear: could not access termcap database");
		return 1;
	}
	clear_string = tgetstr("cl",NULL);
	if(clear_string)
		tputs(clear_string, 1, ft_outc);
	return 0;
}