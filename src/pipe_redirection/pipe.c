#include "minishell.h"

// pid_t	child_process(t_vars *vars, t_command *current_cmd)
// {
// 	pid_t		child_pid;
	
// 	pipe(current_cmd->fd);
// 	child_pid = fork();
// 	if (pipe(current_cmd->fd) < 0)
// 		perror("pipe");
// 	dup2(current_cmd->fd[1], 1);
// 	close(current_cmd->fd[0]);
// 	close(current_cmd->fd[1]);

// 	return (child_pid);
// }

int	pipe_flow(int *fd, int inout)
{
	if (dup2(fd[inout], inout) == -1)
		perror("dup2");
	close(fd[0]);
	close(fd[1]);
	return (0);
}
