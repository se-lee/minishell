#include "minishell.h"

void	builtin_exit(t_command *current_cmd)
{
	int	return_value;

	return_value = 0;
	if (current_cmd->command[1] == NULL)
	{
		exit(return_value);
	}
	else if (current_cmd->command[1])
	{
		if (ft_isalnum(current_cmd->command[1]) == 1 && current_cmd->command[2] == NULL)
		{
			return_value = atoi(current_cmd->command[1]);
			if (current_cmd->pipe == 0)
				printf("exit\n");
			exit(return_value);
		}
		else if (ft_isalnum(current_cmd->command[1]) == 1 && current_cmd->command[2] != NULL)
			printf("exit: too many arguments\n");
		else
		{
			printf("exit : %s: numeric argument required\n");
			exit(255);
		}
	}
}
