#include "../../include/minishell.h"

/*
- [cd] with only a relative or absolute path
parse the second argument (after "cd")

check ~ or . or .. and /

error: if path is not valid

- getcwd: get working directory pathname
- chdir: change current working directory
		int chdir(const char *path);

0) / + pathname (absolute path)
1) ~/ + pathname
2) ../ + pathname   
fainéante
3) ./ or nothing + pathname

get pathname with getcw function

char *getcwd(char *buf, size_t size);
*/

void	builtin_cd(t_vars *vars);

// int main()
// {
// 	char	*pathname;
// 	int		path_len = 100;

// 	ft_memset(pathname, '\0', path_len);
// printf("before: %s\n", pathname);
// 	getcwd(pathname, path_len);
// printf("current path: %s\n", pathname);

// 	chdir("/Users/seoyounglee/Documents/[42]/minishell/src/");
// 	getcwd(pathname, path_len);
// printf("new path: %s\n", pathname);

// 	return (0);
// }

// ".." split with '/' and erase the last string (element) in array
//




