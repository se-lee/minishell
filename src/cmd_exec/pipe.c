#include "minishell.h"

void	pipe_loop(t_vars *vars, t_command *current_cmd, int input)
{
	if (pipe(current_cmd->fd) < 0)
		perror("pipe");
	launch_commands(vars, current_cmd, input, current_cmd->fd[1]);
}

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

void	fd_close(int input, int output)
{
	if (input != 0)
		close(input);
	if (output != 1)
		close(output);
}
