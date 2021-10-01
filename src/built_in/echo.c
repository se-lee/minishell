#include "minishell.h"

/*
- [echo] with option -n
when 'echo' is called with argument
print the arguments 

- show new line character with -n option
- where to put spaces

*/

void	builtin_echo(t_vars *vars, t_token *current_token) 
{
	int	n_option;

	n_option = 0;
	current_token = current_token->next;
	if (current_token && ft_piperedirect(current_token->token_type) == 0)
	{
		if (current_token->buffer.str[0] == '-')
		{
			//Doit gere "-nnnnnnnnnnnnnnn -n -n -n"
			while (ft_strncmp(current_token->buffer.str, "-n", 3) == 0)
			{
				n_option = 1;
				current_token = current_token->next;
			}
		}
		while (current_token && ft_piperedirect(current_token->token_type) == 0)
		{
			printf("%s", current_token->buffer.str);
			current_token = current_token->next;
			if (current_token && ft_piperedirect(current_token->token_type) == 0)
				printf(" ");
		}
	}
	if (n_option == 0)
		printf("\n");
}
