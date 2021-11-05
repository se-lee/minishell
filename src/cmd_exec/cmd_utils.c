#include "minishell.h"

char	*search_current_dir(char *command)
{
	char	path[MAXPATHLEN];
	char	*temp;
	char	*temp2;

	if (getcwd(path, MAXPATHLEN) != NULL)
	{
		temp = ft_strjoin_char(path, '/');
		temp2 = ft_strjoin(temp, command);
		free(temp);
		if (access(temp2, X_OK) == 0)
			return (temp2);
		free(temp2);
	}
	return (NULL);
}

char	*get_command_path(t_envlist *envp, char *command)
{
	char	**path_sep;
	char	*path;
	int		i;

	path = get_env_value(envp, "PATH", 0);
	if (path == NULL)
	{
		perror("path invalid");
		free(path);
	}
	path_sep = ft_split(path, ':');
	path = NULL;
	if (ft_strchr(command, '/') != 0)
		path = ft_strdup(command);
	else if (ft_strncmp("./", command, 2) == 0)
		path = search_current_dir(&command[2]);
	i = 0;
	while (path_sep[i])
	{
		ft_append(&path_sep[i], "/");
		ft_append(&path_sep[i], command);
		if (access(path_sep[i], X_OK) == 0)
		{
			path = ft_strdup(path_sep[i]);
			free_array(path_sep);
			return (path);
		}
		i++;
	}
	free_array(path_sep);
	return (path);
}

int	count_command(t_command *cmd)
{
	int			count;
	t_command	*current_cmd;

	current_cmd = cmd;
	count = 0;
	while (current_cmd)
	{
		count++;
		current_cmd = current_cmd->next;
	}
	return (count);
}

int	command_is_builtin(char **command)
{
	if ((ft_strcmp(command[0], "cd") == 0)
		|| (ft_strcmp(command[0], "echo") == 0)
		|| (ft_strcmp(command[0], "env") == 0)
		|| (ft_strcmp(command[0], "exit") == 0)
		|| (ft_strcmp(command[0], "export") == 0)
		|| (ft_strcmp(command[0], "pwd") == 0)
		|| (ft_strcmp(command[0], "unset") == 0))
		return (TRUE);
	else
		return (FALSE);
}

char	*get_env_value(t_envlist *envp, char *env_name, int return_value)
{
	t_envlist	*current_env;
	char		*value;

	current_env = envp;
	value = NULL;
	while (current_env)
	{
		if (ft_strcmp(current_env->name, env_name) == 0 && current_env->value)
			value = ft_strdup(current_env->value);
		current_env = current_env->next;
	}
	if (ft_strcmp(env_name, "?") == 0)
		value = ft_itoa(return_value);
	return (value);
}

char	**envlist_to_char_array(t_envlist *envp)
{
	char		**env_arr;
	t_envlist	*current_env;
	int			i;

	i = 0;
	current_env = envp;
	env_arr = protected_malloc((envlist_count(envp) + 1), sizeof(char *));
	while (current_env)
	{
		env_arr[i] = ft_strjoin(current_env->name, "=");
		if (current_env->value)
			ft_append(&env_arr[i], current_env->value);
		current_env = current_env->next;
		i++;
	}
	env_arr[i] = NULL;
	return (env_arr);
}
