#include "minishell.h"

void	ft_append(char **a, char *str)
{
	char	*result;

	result = ft_strjoin(*a, str);
	free(*a);
	*a = result;
}

void	builtin_exec(t_vars *vars, char **envp) //+ t_token *current_token
{
	char *command;
	int	len_command;
	t_token	*current_token;

	command = vars->first->buffer.str;
	len_command = ft_strlen(command);
	current_token = vars->first;
	// if (ft_strcmp(command, "cd") == 0)
		// builtin_cd(vars);
	if (ft_strcmp(command, "echo") == 0)
		builtin_echo(vars, current_token->next);
	else if (ft_strcmp(command, "env") == 0)
		builtin_env(vars);
	else if (ft_strcmp(command, "exit") == 0)
		builtin_exit();
	else if (ft_strcmp(command, "export") == 0)
		builtin_export(vars, current_token->next);
	else if (ft_strcmp(command, "pwd") == 0)
		builtin_pwd();
	else if (ft_strcmp(command, "unset") == 0)
		builtin_unset(vars, current_token->next);
	else
		command_exec(vars);
}

/*

PATH=/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/share/dotnet:~/.dotnet/tools:/Library/Apple/usr/bin:/Library/Frameworks/Mono.framework/Versions/Current/Commands

*/

char	**get_command_path(t_vars *vars, t_token *current_token)
{
	t_envlist	*current_env;
	char		**path_sep;
	char		*path;
	int			i;

	path = NULL;
	current_env = vars->envp;
	while (current_env != NULL)
	{
		if (ft_strncmp(current_env->str, "PATH=", 5) == 0)
			path = current_env->str;
		current_env = current_env->next;
	}
	path = ft_substr(path, 5, (ft_strlen(path) - 5));
printf("path: %s\n", path);
	path_sep = ft_split(path, ':');
	free(path);
	i = 0;
	while (path_sep[i] != NULL)
	{
		ft_append(path_sep[i], "/");
		ft_append(path_sep[i], current_token->buffer.str);
printf("path_sep[i]* %s\n", path_sep[i]);
		i++;
	}
	return (path_sep);
}

// execution of commands other than builtin commands ---- execve
void	command_exec(t_vars *vars)
{
	t_token	*current_token;
	char	*command;
	char	**path;
	int		i;

	path = get_command_path(vars, current_token);
	command = vars->first->buffer.str;
	i = 0;
	while (path[i])
	{
		execve(path[i], &command, envp);
		i++;
	}
}
