#include "minishell.h"

void	ft_append(char **a, char *str)
{
	char	*result;

	result = ft_strjoin(*a, str);
	free(*a);
	*a = result;
}

int	count_number_str_in_list(t_vars *vars)
{
	int		count;
	t_token	*index;

	index = vars->first;
	count = 0;
	while (index)
	{
		if (index->token_type == WORD)
			count++;
		index = index->next;
	}
	return (count);
}

static char	**get_command(t_vars *vars, t_token *current_token)
{
	char	**command;
	int		str_count;
	int		i;

	str_count = count_number_str_in_list(vars);
	command = malloc(sizeof(char *) * (str_count + 1));
	i = 0;
	while (current_token && ft_piperedirect(current_token->token_type) == 0)
	{
		if (current_token->token_type == WORD)
		{
			command[i] = ft_strdup(current_token->buffer.str);
			i++;
		}
		current_token = current_token->next;
	}
	command[i] = NULL;
	return (command);
}

static char	**get_command_path(t_vars *vars, char *command)
{
	char	**path_sep;
	char	*path;
	int		i;

	path = getenv("PATH");
	if (path == NULL)
		perror("path invalid");
	path_sep = ft_split(path, ':');
	i = 0;
	while (path_sep[i] != NULL)
	{
		ft_append(&path_sep[i], "/");
		ft_append(&path_sep[i], command);
		i++;
	}
	return (path_sep);
}

void	execute_other_cmd(t_vars *vars, char **envp)
{
	t_token	*current_token;
	char	**command_arr;
	char	**path_sep;
	int		i;

	current_token = vars->first;
	command_arr = get_command(vars, current_token);
	path_sep = get_command_path(vars, command_arr[0]);
	i = 0;
	while (path_sep[i])
	{
		execve(path_sep[i], command_arr, envp);
		i++;
	}
	perror(command_arr[0]);
}

void	execute_command(t_vars *vars, char **envp)
{
	char	*command;
	t_token	*current_token;

	command = vars->first->buffer.str;
	current_token = vars->first;
	if (ft_strcmp(command, "cd") == 0)
		builtin_cd(vars, current_token);
	else if (ft_strcmp(command, "echo") == 0)
		builtin_echo(vars, current_token);
	else if (ft_strcmp(command, "env") == 0)
		builtin_env(vars);
	else if (ft_strcmp(command, "exit") == 0)
		builtin_exit();
	else if (ft_strcmp(command, "export") == 0)
		builtin_export(vars, current_token);
	else if (ft_strcmp(command, "pwd") == 0)
		builtin_pwd();
	else if (ft_strcmp(command, "unset") == 0)
		builtin_unset(vars, current_token->next);
	else
		execute_other_cmd(vars, envp);
}
