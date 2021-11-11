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
	if (path == NULL || ft_strncmp("/", command, 1) == 0)
		return (command);
	if (ft_strncmp("./", command, 2) == 0)
		return (search_current_dir(&command[2]));
	path_sep = ft_split(path, ':');
	path = NULL;
	if (ft_strchr(command, '/') != 0)
		path = ft_strdup(command);
	i = 0;
	while (path_sep[i])
	{
		ft_append(&path_sep[i], "/");
		ft_append(&path_sep[i], command);
		if (access(path_sep[i], X_OK) == 0)
			return (path_sep[i]);
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
