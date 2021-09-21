#include "minishell.h"

int	isspecial(char c)
{
	if ((c == '"') || (c == '\'') || (c == '$')
	|| (c == '<') || (c == '>') || (c == '|'))
		return (1);
	else
		return (0);
}
