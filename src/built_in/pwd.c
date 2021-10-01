#include "minishell.h"

void	builtin_pwd(void)
{
	char path[MAXPATHLEN];
	getcwd(path, MAXPATHLEN);
	printf("%s\n", path);
	exit(0);
}
