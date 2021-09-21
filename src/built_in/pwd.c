#include "../../include/minishell.h"
#include <errno.h>
#include <sys/param.h>

// typedef struct s_path
// {
// 	char	*name;
// 	int		length;
// } t_path;

// void	init_path(t_path *path)
// {
// printf("!");
// 	path->length = 100;
// 	ft_memset(path->name, '\0', path->length);
// }

void	builtin_pwd()//t_path *path)
{
//	init_path(path);
	char path[MAXPATHLEN];
	char *res;

	res = getcwd(path, MAXPATHLEN);
	printf("pwd: %s\n", res);
}

int main()
{
//	t_path *path;
	builtin_pwd();

	return (0);
}
