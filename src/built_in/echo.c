#include "../../include/minishell.h"

/*
- [echo] with option -n

when 'echo' is called with argument
print the arguments 

- show new line character with -n option
- where to put spaces

*/

// declare *current_token = vars->first
// to move (increment) current_token = current_token->next;
// think about using "Token type"

void	builtin_echo(t_vars *vars, t_token *current_token) 
{
	int i;
	int	n_option;
	char *first_arg;
	char *second_arg;

	first_arg = vars->first->next->next->buffer.str;
	second_arg = vars->first->next->next->buffer.str;
	// if (ft_strncmp(first_arg, "-n", 2) == 0)
	// {
	// 	n_option = 1;
	// 	i = 2;
	// }
	// else
	// {
	// 	n_option = 0;
	// 	i = 1;
	// }
	while(current_token)
	{
		if (current_token->token_type == WORD)
			printf("%s", current_token->buffer.str);
		if (current_token->token_type == SPACE_SIGN &&
				current_token->next->token_type != SPACE_SIGN)
			printf(" ");
		current_token = current_token->next;
	}
	// if (n_option == 0)
		printf("\n");
}
