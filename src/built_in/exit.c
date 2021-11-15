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

void	remove_space(char **str)
{
	char	*temp;

	temp = ft_strtrim(*str, " ");
	free(*str);
	*str = temp;
}

void	display_error_and_exit(t_command *current_cmd, char *message, int exit_status)
{
	display_cmd_error(current_cmd, message, TRUE);
	exit(exit_status);
}

void	builtin_exit(t_vars *vars, t_command *current_cmd)
{
	if (current_cmd->command[1] == NULL)
	{
		printf("exit\n");
		exit(vars->return_value);
	}
	else if (current_cmd->command[1])
	{
		if (current_cmd->command[1][0] == ' ') //Added
			remove_space(&current_cmd->command[1]);
		if (strisnum(current_cmd->command[1]) == 1
			&& current_cmd->command[2] == NULL)
		{
			vars->return_value = ft_atoi(current_cmd->command[1]); //atoi -> ft_atoi
printf("ret:%d\n", vars->return_value);
			if (vars->return_value > 255 || vars->return_value < 0)
				vars->return_value = vars->return_value % 256;
			if (count_command(vars->cmd) == 1) //current_cmd->pipe == 0)
				printf("exit\n");
			exit(vars->return_value);
		}
		else if (strisnum(current_cmd->command[1]) == 1
			&& current_cmd->command[2] != NULL)
			display_cmd_error(current_cmd, "too many arguments", FALSE);
		else
			display_error_and_exit(current_cmd, "numeric argument required", 255);
	}
}

		// {
		// 	display_cmd_error(current_cmd, "numeric argument required", TRUE);
		// 	exit(255);
		// }