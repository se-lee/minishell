#include "minishell.h"

void	fd_dup_and_close(int input, int output)
{
	if (input != 0)
	{
		if (dup2(input, 0) < 0)
			perror("dup2");
		close(input);
	}
	if (output != 1)
	{
		if (dup2(output, 1) < 0)
			perror("dup2");
		close(output);
	}
}

void	pipe_and_launch_command(t_vars *vars, t_command *current_cmd,
			int input, int to_close)
{
	if (pipe(current_cmd->fd) < 0)
		ft_putendl_fd("error: pipe", 2);
	if (!to_close)
		to_close = current_cmd->fd[0];
	launch_commands(vars, current_cmd, (int [2]){input, current_cmd->fd[1]}, to_close);
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
