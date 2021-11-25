#include "minishell.h"

void	run_command_builtin(t_vars *vars, t_command *current_cmd)
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
}

int	check_stat_and_access(t_vars *vars, t_command *current_cmd, char *path)
{
	struct stat	buff;

	if (path != NULL)
	{
		if (stat(path, &buff) < 0)
		{
			display_cmd_error(current_cmd, "No such file or directory", FALSE);
			vars->return_value = 127;
			return (FALSE);
		}
		else if (S_ISDIR(buff.st_mode))
		{
			printf("a\n");
			display_cmd_error(current_cmd, "is a directory", FALSE);
			vars->return_value = 126;
			return (FALSE);
		}
		else if (access(path, X_OK) < 0)
		{
			printf("b\n");
			display_cmd_error(current_cmd, "Permission denied", FALSE);
			vars->return_value = 126;
			return (FALSE);
		}
	}
	return (TRUE);
}

void	run_command_non_builtin(t_vars *vars,
	t_envlist *envlist, t_command *current_cmd)
{
	char		*path;
	char		**env;

	env = envlist_to_char_array(envlist);
	path = get_command_path(envlist, current_cmd->command[0]);
	// if (stat(path, &buff) < 0 && path != NULL)
	// {
	// 	display_cmd_error(current_cmd, "No such file or directory", FALSE);
	// 	vars->return_value = 127;
	// }
	if (check_stat_and_access(vars, current_cmd, path) == FALSE)
		exit (vars->return_value);
	if (path != NULL)
	{
		// if (S_ISDIR(buff.st_mode)) //&& current_cmd->command == NULL)
		// {
		// 	printf("a\n");
		// 	display_cmd_error(current_cmd, "is a directory", FALSE);
		// 	vars->return_value = 126;
		// }
		// else if (access(path, X_OK) < 0)
		// {
		// 	printf("b\n");
		// 	display_cmd_error(current_cmd, "Permission denied", FALSE);
		// 	vars->return_value = 126;
		// }
		if (execve(path, current_cmd->command, env) < 0)
		{
			printf("c\n");
			ft_putstr_fd("minishell: ", 2);
			perror(current_cmd->command[0]);
			vars->return_value = 126;
		}
	}
	else
	{
		display_cmd_error(current_cmd, "command not found", FALSE);
		vars->return_value = 127;
	}
	free(path);
	exit(vars->return_value);
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
