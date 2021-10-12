#include "minishell.h"

int	pipe_flow(int *fd, int inout)
{
	if (dup2(fd[inout], inout) == -1)
		perror("dup2");
	close(fd[0]);
	close(fd[1]);
	return (0);
}

void	child_processes(t_vars *vars, t_command *current_cmd, int i, int cmd_count)
{
	pid_t	child;
	int		fd[2];

	if (pipe(fd) < 0)
		perror("pipe");
	child = fork();
	if (child < 0)
		perror("fork");
	if (child == 0)
	{
		dup2(fd[1], 1);
		close(fd[0]);
		close(fd[1]);
		if (command_is_builtin(current_cmd->command) == TRUE)
			run_command_builtin(vars, current_cmd);
		else
			run_command_non_builtin(vars->envp, current_cmd);
	}
	else //parent
	{
		waitpid(child, NULL, 0);
		dup2(fd[0], 0);
		close(fd[0]);
		close(fd[1]);
		if (command_is_builtin(current_cmd->command) == TRUE)
			run_command_builtin(vars, current_cmd);
		else
			run_command_non_builtin(vars->envp, current_cmd);
	}
}

