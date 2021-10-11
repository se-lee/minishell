#include "minishell.h"

int	pipe_flow(int *fd, int inout)
{
	if (dup2(fd[inout], inout) == -1)
		perror("dup2");
	close(fd[0]);
	close(fd[1]);
	return (0);
}

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
			if (i == 0) //first command
				pipe_flow(current_cmd->fd, OUT);
			else if (i == cmd_count - 1 && cmd_count != 1) //last command
				pipe_flow(current_cmd->prev->fd, IN);
			else //middle command(s)
			{
				pipe_flow(current_cmd->prev->fd, IN);
				pipe_flow(current_cmd->fd, OUT);
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

