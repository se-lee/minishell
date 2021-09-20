#include "minishell.h"

int	isspecial(char c)
{
	if (c == '"' || c == '\'' || c == '$'
		|| c == '<' || c == '>' || c == '|')
		return (1);
	return (0);
}

char	**tokenization(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (isspecial(str[i]))
		{
			
		}
		i++;
	}
}

void parsing(t_vars *vars, char *str)
{
	char	**tokens;
	tokens = tokenization(str);
	return ;
}
