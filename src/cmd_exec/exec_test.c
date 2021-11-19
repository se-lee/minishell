#include "minishell.h"

void	execve_command(t_vars *vars, t_command *current_cmd, t_envlist *envlist)
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

void	run_command(t_vars *vars, t_command *current_cmd, t_envlist *envlist)
{
	char	*command;

	command = current_cmd->command[0];
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
		execve_command(vars, current_cmd, envlist);
}

void	execute_command(t_vars *vars, t_envlist *envlist)
{
	t_command	*current_cmd;

	if (!vars->cmd)
		return ;
	current_cmd = vars->cmd;
	tcsetattr(STDIN_FILENO, TCSANOW, &vars->saved_termios);
	run_command(vars, current_cmd, envlist);
}

/*
run_command_builtinのなかにNon-builtinのファンクションも実行するようにする。
builtin 以外の全部 

in non built in execution function,
do fork execution -> and run execve inside the fork (child process)

*/
