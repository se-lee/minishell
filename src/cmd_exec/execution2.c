#include "minishell.h"

void	run_command_and_exit(t_vars *vars, t_command *current_cmd)
{
	if (command_is_builtin(current_cmd->command) == TRUE)
	{
		run_command_builtin(vars, current_cmd);
		exit(0);
	}
	else
	{
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

void	fork_and_run_command(t_vars *vars, t_command *current_cmd, int input, int output, int to_close)
{
	pid_t	child;
	child = fork();

	if (child < 0)
		perror("fork");
	if (child == 0)
	{
		signal(SIGINT, sigchild);
		signal(SIGQUIT, sigchild);
		redirection(vars, current_cmd);
		fd_dup_and_close(input, output);
		if (to_close)
			close(to_close);
		run_command_and_exit(vars, current_cmd);
	}
	else
	{
		signal(SIGINT, sigmain);
		signal(SIGQUIT, sigmain);
		fd_close(input, output);
	}
}

void	run_command_with_pipe(t_vars *vars, t_command *current_cmd)
{
	int		i;
	int		output;
	int		input;
	int		to_close;
	
	i = 0;
	output = 1;
	input = 0;
	to_close = 0;
	while (i < count_command(vars->cmd) - 1)
	{
		if (pipe(current_cmd->fd) < 0)
			perror ("pipe");
		if (!to_close)
			to_close = current_cmd->fd[0];
		fork_and_run_command(vars, current_cmd, input, output, to_close);
		to_close = 0;
		input = current_cmd->fd[0];
		current_cmd = current_cmd->next;
		i++;
	}
	fork_and_run_command(vars, current_cmd, input, output, to_close);
}

void execute_command(t_vars *vars)
{
	t_command	*current_cmd;
	int			i;
	pid_t		child;

	i = 0;
	child = 0;
	current_cmd = vars->cmd;
	tcsetattr(STDIN_FILENO, TCSANOW, &vars->saved_termios);
	if (count_heredoc(vars) > 0)
		update_heredoc(vars);
	if (!current_cmd->pipe)
		run_command_no_pipe(vars, current_cmd);
	else
		run_command_with_pipe(vars, current_cmd);
	i = 0;
	while (i < count_command(vars->cmd))
	{
		waitpid(child, NULL, 0);
		i++;
	}
}
