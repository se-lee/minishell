#include "minishell.h"

/*
- [echo] with option -n
when 'echo' is called with argument
print the arguments 

- show new line character with -n option
- where to put spaces

*/

int	echo_n(char *str)
{
	int	i;

	if (str[0] != '-')
		return (0);
	i = 1;
	while (str[i] && str[i] == 'n')
		i++;
	if (str[i])
		return (0);
	return (1);
}

void	builtin_echo(t_vars *vars, t_token *current_token)
{
	int	n_option;

	n_option = 0;
	current_token = current_token->next;
	if (current_token && ft_piperedirect(current_token->token_type) == 0)
	{
		if (current_token->buffer.str[0] == '-')
		{
			while (echo_n(current_token->buffer.str) == 1)
			{
				n_option = 1;
				current_token = current_token->next;
			}
		}
		while (current_token && ft_piperedirect(current_token->token_type) == 0)
		{
			printf("%s", current_token->buffer.str);
			current_token = current_token->next;
			if (current_token
				&& ft_piperedirect(current_token->token_type) == 0)
				printf(" ");
		}
	}
	if (n_option == 0)
		printf("\n");
}
