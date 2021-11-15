#include "minishell.h"

/*
void	run_command_builtin(t_vars *vars, t_command *current_cmd)
{
	char	*command;


	command = current_cmd->command[0];
	if (ft_strcmp(command, "cd") == 0)
		builtin_cd(vars, current_cmd);
	else if (ft_strcmp(command, "echo") == 0)
		builtin_echo(current_cmd);
	else if (ft_strcmp(command, "env") == 0)
		builtin_env(vars);
	else if (ft_strcmp(command, "exit") == 0)
		builtin_exit(vars, current_cmd);
	else if (ft_strcmp(command, "export") == 0)
		builtin_export(vars, current_cmd);
	else if (ft_strcmp(command, "pwd") == 0)
		builtin_pwd();
	else if (ft_strcmp(command, "unset") == 0)
		builtin_unset(vars, current_cmd, 0);
}
*/

int	run_command_builtin(t_vars *vars, t_command *current_cmd)
{
	char	*command;
	int		return_value;

	command = current_cmd->command[0];
	return_value = 0;
	if (ft_strcmp(command, "cd") == 0)
		return_value = builtin_cd(vars, current_cmd);
	else if (ft_strcmp(command, "echo") == 0)
		builtin_echo(current_cmd);
	else if (ft_strcmp(command, "env") == 0)
		builtin_env(vars);
	else if (ft_strcmp(command, "exit") == 0)
		builtin_exit(vars, current_cmd);
	else if (ft_strcmp(command, "export") == 0)
		builtin_export(vars, current_cmd);
	else if (ft_strcmp(command, "pwd") == 0)
		builtin_pwd();
	else if (ft_strcmp(command, "unset") == 0)
		builtin_unset(vars, current_cmd, 0);
	return (return_value);
}


int	run_command_non_builtin(t_vars *vars, t_envlist *envlist, t_command *current_cmd)
{
	char	*path;
	char	**env;

	env = envlist_to_char_array(envlist);
	path = get_command_path(envlist, current_cmd->command[0]);
	if (path != NULL)
	{
		if (execve(path, current_cmd->command, env) < 0)
		{
			perror(current_cmd->command[0]);
			free(path);
			vars->return_value = 127;
			exit(vars->return_value);
		}
	}
	else if (ft_strncmp(current_cmd->command[0], "|", 1) == 0)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `|'", 2);
		free(path);
		vars->return_value = 127;
		exit(vars->return_value);
	}
	else
	{
		display_cmd_error(current_cmd, "command not found", FALSE);
		free(path);
		vars->return_value = 127;
		exit(vars->return_value);
	}
		return (0);
}

void	run_command_and_exit(t_vars *vars, t_command *current_cmd)
{
	if (command_is_builtin(current_cmd->command) == TRUE)
	{
		run_command_builtin(vars, current_cmd);
		exit(vars->return_value);
	}
	else
	{
		run_command_non_builtin(vars, vars->envp, current_cmd);
		exit(vars->return_value);
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
		run_command_non_builtin(vars, vars->envp, current_cmd);
	}
}

void	pipe_get_next_cmd(t_command *current_cmd)
{
	char		*line;

	current_cmd->next = protected_malloc(1, sizeof(t_command));
	current_cmd->next->next = NULL;
	line = NULL;
	ft_putstr_fd("> ", OUT);
	get_next_line(IN, &line);
	current_cmd->next->command = ft_split(line, ' ');
	free(line);
}
