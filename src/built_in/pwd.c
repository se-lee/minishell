#include "../../include/minishell.h"
#include <errno.h>
#include <sys/param.h>

typedef struct s_path
{
	char	name[MAXPATHLEN];
} t_path;


void	builtin_pwd()//t_path *path)
{
	char path[MAXPATHLEN];
	getcwd(path, MAXPATHLEN);
	printf("%s\n", path);
}

// int main(int argc, char **argv)
// {
// 	(void)argc;
// 	if (ft_strcmp(argv[1], "pwd") == 0)
// 		builtin_pwd();
// 	else if (ft_strcmp(argv[1], "echo") == 0)
// 		builtin_echo(argv);
// 	return (0);
// }
