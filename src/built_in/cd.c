#include "minishell.h"

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
3) ./ or nothing + pathname

get pathname with getcw function

char *getcwd(char *buf, size_t size);
	getcwd(pathname, path_len);
	chdir("/Users/seoyounglee/Documents/[42]/minishell/src/");
	getcwd(pathname, path_len);
*/

static char	*get_path(t_vars *vars)
{
	char *path;
	return (path);
}

void	builtin_cd(t_vars *vars, t_token *current_token)
{
	char	*path;

	current_token = current_token->next;
	// path = getcwd();
printf("path:%s\n", path);
	chdir(path);

}
