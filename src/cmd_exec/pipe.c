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

// void	run_command_with_pipe(t_vars *vars, t_command *current_cmd)
// {
// 	int	fd[2];
// 	int	input;
// 	int	output;

// 	input = 0;
// 	output = 1;
// 	if (pipe(fd) < 0)
// 		perror("pipe");
// 	launch_command(vars, current_cmd, input, fd[1]);
// 	input = fd[0];

// }