#include "minishell.h"

void	run_command_and_exit(t_vars *vars, t_command *current_cmd)
{
	if (command_is_builtin(current_cmd->command) == TRUE)
	{
		redirection(vars, current_cmd);
		run_command_builtin(vars, current_cmd);
		exit(0);
	}
	else
	{
		redirection(vars, current_cmd);
		run_command_non_builtin(vars->envp, current_cmd);
		exit(0);
	}
}

void	redirect_and_run_cmd(t_vars *vars, t_command *current_cmd, int builtin)
{
	if (builtin == TRUE)
	{
		redirection(vars, current_cmd);
		run_command_builtin(vars, current_cmd);
		exit(0);
	}
	else
	{
		redirection(vars, current_cmd);
		run_command_non_builtin(vars->envp, current_cmd);		
	}
}

void	wait_loop(int command_count, pid_t child)
{
	int		i;

	i = 0;
	while (i < command_count)
	{
		waitpid(child, NULL, 0);
		i++;
	}
}

