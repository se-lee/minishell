#include "minishell.h"

/*
command->redirect_leftが2だったら→ヒアドッグ

<< のあとにくる文字列と同じ文字列が入力されたら
入力の読み込み終了

Signals : ctrl-D


*/

char	*get_delimiter(t_vars *vars)
{
	char	*delimiter;
	t_redirect	*current_in;

	current_in = vars->in;
	if (current_in->arrow_num == 2 && current_in->filename)
		delimiter = ft_strdup(current_in->filename);
	else
		delimiter = NULL;
	return (delimiter);
}

void		heredoc(t_vars *vars)
{
	int		fd;
	char	*delimiter;
	char	*line;

	fd = open(".heredoc", O_TRUNC | O_CREAT | O_WRONLY, 0644);
	delimiter = get_delimiter(vars);
printf("delimiter:%s\n", delimiter);
	if (fd < 0)
		perror("heredoc_file");
	line = readline("> ");
	while (ft_strcmp(line, delimiter))
	{
		ft_putendl_fd(line, fd);
		free(line);
		line = readline("> ");
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
		perror("dup2");
	close(fd);
	exit(0);
	return ;
/*d
remove temporary file after use (unlink function?)
*/
}
