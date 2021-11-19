#include "minishell.h"

void	fork_execute_command(t_vars *vars, t_command *current_cmd, t_envlist *envlist)
{
	char	*path;
	char	**env;
	int		temp;
	pid_t	child;
	int		status;

	status = 0;
	env = envlist_to_char_array(envlist);
	path = get_command_path(envlist, current_cmd->command[0]);
	if (ft_strncmp(current_cmd->command[0], "|", 1) == 0)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `|'", 2);
		free(path);
		g_vars->return_value = 127;
		exit(g_vars->return_value);
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
			printf("ret:%d\n", current_cmd->exit_status);
				free(path);
				exit(current_cmd->exit_status);
			}
		}
		else
			waitpid(child, &status, 0);
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
		vars->return_value = builtin_cd(vars, current_cmd);
	else if (ft_strcmp(command, "echo") == 0)
		vars->return_value = builtin_echo(current_cmd);
	else if (ft_strcmp(command, "env") == 0)
		vars->return_value = builtin_env(vars);
	else if (ft_strcmp(command, "exit") == 0)
		builtin_exit(vars, current_cmd);
	else if (ft_strcmp(command, "export") == 0)
		vars->return_value = builtin_export(vars, current_cmd);
	else if (ft_strcmp(command, "pwd") == 0)
		vars->return_value = builtin_pwd();
	else if (ft_strcmp(command, "unset") == 0)
		vars->return_value = builtin_unset(vars, current_cmd, 0);
	else
		fork_execute_command(vars, current_cmd, vars->envp);
}

void	pipe_fork_child(t_vars *vars, t_command *current_cmd)
{
	if (pipe(current_cmd->fd) < 0)
		ft_putendl_fd("error: pipe", 2);
	current_cmd->pid = fork();
	if  (current_cmd->pid < 0)
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

/*
void	execute_command(t_vars *vars)
{
	t_command	*current_cmd;

	if (!vars->cmd)
		return ;
	current_cmd = vars->cmd;
	tcsetattr(STDIN_FILENO, TCSANOW, &vars->saved_termios);
	if (!current_cmd->pipe)
		run_command(vars, current_cmd);
	else
	{
		while(current_cmd->next != NULL)
		{
			pipe_fork_child(vars, current_cmd);
			current_cmd = current_cmd->next;
		}
		pipe_fork_child(vars, current_cmd);
	}
}
*/