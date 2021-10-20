#include "minishell.h"

void	builtin_pwd(void)
{
	char	path[MAXPATHLEN];

	if (getcwd(path, MAXPATHLEN) != NULL)
		printf("%s\n", path);
	else
		perror("pwd");
}
