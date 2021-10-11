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


	if (pipe(current_cmd->fd) < 0)
			perror("pipe");
	child = fork();
if (child < 0)
		perror("fork");
	if (child == 0)
	{
			if (i == 0) //first command
			{
	printf("1i:%d\n", i);
	printf("1\n");
				dup2(0, 0);
				dup2(current_cmd->fd[1], 1);
				close(current_cmd->fd[0]);
				close(current_cmd->fd[1]);
			}
			else if (i == cmd_count - 1) //last command
			{
	printf("2i:%d\n", i);
	printf("2\n");
				dup2(1, 1);
				dup2(current_cmd->prev->fd[1], 1);
				close(current_cmd->prev->fd[0]);
				close(current_cmd->prev->fd[1]);
			}
			else //middle command(s)
			{
	printf("3i:%d\n", i);
	printf("3\n");
				dup2(current_cmd->prev->fd[0], 0);
				dup2(current_cmd->fd[1], 1);
				close(current_cmd->prev->fd[0]);
				close(current_cmd->prev->fd[1]);
				close(current_cmd->fd[0]);
				close(current_cmd->fd[1]);			
			}
			if (command_is_builtin(current_cmd->command) == TRUE)
{
printf("cmd:%s\n", current_cmd->command[0]);
				run_command_builtin(vars, current_cmd);
}
			else
				run_command_non_builtin(vars->envp, current_cmd);
		}

	// else
	// {
	// 	close(current_cmd->prev->fd[0]);
	// 	close(current_cmd->prev->fd[1]);
	// }
}

//the parent closes fd that are not used
//
