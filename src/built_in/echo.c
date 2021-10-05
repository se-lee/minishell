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

void	builtin_echo(t_vars *vars, t_command *current_cmd)
{
	int	n_option;
	int	i;

	n_option = 0;
	if (current_cmd && current_cmd->command[1] != NULL)
	{
		if (current_cmd->command[1][0] == '-')
		{
			i = 1;
			while (echo_n(current_cmd->command[i]) == 1)
			{
				n_option = 1;
				i++;
			}
		}
		while (current_cmd->command[i])
		{
			printf("%s", current_cmd->command[i]);
			i++;
			if (current_cmd->command[i])
				printf(" ");
		}
	}
	if (n_option == 0)
		printf("\n");
}
