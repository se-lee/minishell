#include "minishell.h"

int	strisnum(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

void	builtin_exit(t_vars *vars, t_command *current_cmd)
{
	if (current_cmd->command[1] == NULL)
		exit(vars->return_value);
	else if (current_cmd->command[1])
	{
		if (strisnum(current_cmd->command[1]) == 1
			&& current_cmd->command[2] == NULL)
		{
			vars->return_value = atoi(current_cmd->command[1]);
			if (vars->return_value > 255 || vars->return_value < 0)
				vars->return_value = vars->return_value % 256;
			if (current_cmd->pipe == 0)
				printf("exit\n");
			exit(vars->return_value);
		}
		else if (strisnum(current_cmd->command[1]) == 1
			&& current_cmd->command[2] != NULL)
			display_cmd_error(current_cmd, "too many arguments", FALSE);
		else
		{
			display_cmd_error(current_cmd, "numeric argument required", TRUE);
			exit(255);
		}
	}
}
