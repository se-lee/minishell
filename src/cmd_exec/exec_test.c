#include "minishell.h"

/*
void	fork_execute_command(t_vars *vars, t_command *current_cmd, t_envlist *envlist)
{
	char	*path;
	char	**env;
	int		temp;
	pid_t	child;
	int		status;

	env = envlist_to_char_array(envlist);
	path = get_command_path(envlist, current_cmd->command[0]);
	if (ft_strncmp(current_cmd->command[0], "|", 1) == 0)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `|'", 2);
		free(path);
		current_cmd->exit_status = 127;
		exit(current_cmd->exit_status);
	}
	else if (path != NULL)
	{
		child = fork();
		if (child == 0)
		{
			redirection(vars, current_cmd);
			if (execve(path, current_cmd->command, env) < 0)
			{
				perror(current_cmd->command[0]);
				current_cmd->exit_status = 127;
				free(path);
				exit(current_cmd->exit_status);
			}
		}
		else
			waitpid(current_cmd->pid, &status, 0);
	}
	else
	{
		display_cmd_error(current_cmd, "command not found", FALSE);
		free(path);
		g_vars->return_value = 127;
	}
}

void	run_command(t_vars *vars, t_command *current_cmd)
{
	char	*command;

	command = current_cmd->command[0];
	if (count_heredoc(vars) > 0)
		update_heredoc(vars);
	if (ft_strcmp(command, "cd") == 0)
		current_cmd->exit_status = builtin_cd(vars, current_cmd);
	else if (ft_strcmp(command, "echo") == 0)
		current_cmd->exit_status = builtin_echo(current_cmd);
	else if (ft_strcmp(command, "env") == 0)
		current_cmd->exit_status = builtin_env(vars);
	else if (ft_strcmp(command, "exit") == 0)
		builtin_exit(vars, current_cmd);
	else if (ft_strcmp(command, "export") == 0)
		current_cmd->exit_status = builtin_export(vars, current_cmd);
	else if (ft_strcmp(command, "pwd") == 0)
		current_cmd->exit_status = builtin_pwd();
	else if (ft_strcmp(command, "unset") == 0)
		current_cmd->exit_status = builtin_unset(vars, current_cmd, 0);
	else
		fork_execute_command(vars, current_cmd, vars->envp);
}

void	pipe_fork_child(t_vars *vars, t_command *current_cmd)
{
	pid_t	child;

	if (pipe(current_cmd->fd) < 0)
		ft_putendl_fd("error: pipe", 2);
	child = fork();
	if (current_cmd->pid < 0)
		perror("fork");
	if (current_cmd->pid == 0)
	{
		fd_dup_and_close(current_cmd->fd[0], current_cmd->fd[1]);
		run_command(vars, current_cmd);
	}
	else
	{
		if (current_cmd->fd[0] != 0)
			close(current_cmd->fd[0]);
		if (current_cmd->fd[1] != 1)
			close(current_cmd->fd[1]);
	}
}

void	execute_command(t_vars *vars)
{
	t_command	*current_cmd;
	int			input;
	int			output;
	int			to_close;

	if (!vars->cmd)
		return ;
	current_cmd = vars->cmd;
	input = 0;
	output = 1;
	to_close = 0;
	tcsetattr(STDIN_FILENO, TCSANOW, &vars->saved_termios);
	if (!current_cmd->pipe)
		run_command(vars, current_cmd);
	else //if there a pipes
	{
printf("cmd_count:%d\n", count_command(vars->cmd));
		if (current_cmd->next == NULL)
			pipe_get_next_cmd(current_cmd);
		while(current_cmd->next != NULL)
		{
			if (pipe(current_cmd->fd) < 0)
				perror("pipe");
			current_cmd->pid = fork();
			if (current_cmd->pid == 0)
			{
//				redirection(vars, current_cmd);
printf("fd[0]:%d    fd[1]:%d\n", current_cmd->fd[0], current_cmd->fd[1]);
				if (!to_close)
					to_close = current_cmd->fd[0];
printf("to_close:%d\n", to_close);
				if (input != 0)
				{
					dup2(input, 0);
					close(input);
				}
				if (output != 1)
				{
					dup2(output, 1);
					close(output);
				}
				if (to_close)
					close(to_close);
				run_command(vars, current_cmd);
			}
			else
			{
				if (current_cmd->fd[0] != 0)
					close(current_cmd->fd[0]);
				if (current_cmd->fd[1] != 1)
					close(current_cmd->fd[1]);
			}
			to_close = 0;
			input = current_cmd->fd[0];
			current_cmd = current_cmd->next;
		}
		redirection(vars, current_cmd);
		run_command(vars, current_cmd);
	}
}
*/