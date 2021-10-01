#include "minishell.h"

void	builtin_cd(t_vars *vars, t_token *current_token)
{
	char	*path;
	int		path_len;

	current_token = current_token->next;
	path = current_token->buffer.str;
printf("path:%s\n", path);
	chdir(path);
}
