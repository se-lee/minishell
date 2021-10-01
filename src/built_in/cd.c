#include "minishell.h"

// handle env variable (rewrite
// cd ~
// cd $___
// cd ~ 1234 --> no error, go to ~
// 

char	*search_home(char **envp)
{
	char	*home_path;
	home_path = getenv("HOME");
	return (home_path);
}

void	builtin_cd(t_vars *vars, t_token *current_token, char **envp)
{
	char	*path;
	char	*path_temp;
	int		path_len;

	current_token = current_token->next;
	path_temp = current_token->buffer.str;
// printf("path_temp:%s\n", path_temp);
	if (path_temp[0] == '~')
		path = ft_strjoin(search_home(envp), ft_substr(path_temp, 1, ft_strlen(path_temp) - 1));
	else
		path = path_temp;
// printf("path:%s\n", path);
	chdir(path);
}
