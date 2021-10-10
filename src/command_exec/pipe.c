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


pid_t	child_processes(t_vars *vars, int cmd_count)
{
	t_command	*current_cmd;
	t_envlist	*envlist;
	int		i;
	pid_t	child;

	current_cmd = vars->cmd;
	envlist = vars->envp;
	i = 0;
	while (i < cmd_count)
	{
		if (pipe(current_cmd->fd) < 0)
			perror("pipe");
		child = fork();
		if (child == 0)
		{
			if (i == 0) //first commands
			{
				dup2(current_cmd->fd[1], 1);
				close(current_cmd->fd[0]);
				close(current_cmd->fd[1]);
			}
			else if (i == cmd_count - 1 && cmd_count != 1) //last command
			{
				dup2(current_cmd->prev->fd[0], 0);
				close(current_cmd->prev->fd[0]);
				close(current_cmd->prev->fd[1]);
			}
			else //middle command
			{
				dup2(current_cmd->prev->fd[0], 0);
				dup2(current_cmd->prev->fd[1], 1);
				close(current_cmd->fd[0]);
				close(current_cmd->fd[1]);			
				close(current_cmd->prev->fd[0]);
				close(current_cmd->prev->fd[1]);			
			}
			if (command_is_builtin(current_cmd->command) == TRUE)
				run_command_builtin(vars, current_cmd);
			else
				run_command_non_builtin(envlist, current_cmd);
		}
		if (current_cmd->next)
			current_cmd = current_cmd->next;
		i++;
	}
	return (child);
}





int	pipe_flow(int *fd, int inout)
{
	if (dup2(fd[inout], inout) == -1)
		perror("dup2");
	close(fd[0]);
	close(fd[1]);
	return (0);
}
