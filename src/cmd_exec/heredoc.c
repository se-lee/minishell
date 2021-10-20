#include "minishell.h"

/*
command->redirect_leftが2だったら→ヒアドッグ

<< のあとにくる文字列と同じ文字列が入力されたら
入力の読み込み終了

入力された内容を構造体に入れる？
ダブル配列にする？

１）まずは簡単なコマンド（パイプなし）

[command] [<<] [delimiter]
display prompt "> " (with space)

*/

char	*get_delimiter(t_command *command)
{
	char	*delimiter;
	if (current_command->command[0] != NULL)
		delimiter = ft_strdup(current_command->command[0]);
	else
		delimiter = NULL;
	return (delimiter);
}

int		heredoc_write(t_command *current_command, char *str)
{
	int		fd[2];
	int		ret;

	if (pipe(fd) < 0)
		return (-1);


}

int	here_doc(char *stop)
{
	char *line;
	int fd[2];

	pipe(fd);
	while (ft_get_next_line(0, &line))
	{
		if (ft_strcmp(line, stop))
		{
			write(fd[1], line, ft_strlen(line));
			write(fd[1], "\n", 1);
			free(line);
		}
	}
	free(line);
	close(fd[1]);
	dup2(fd[0], 0);
	close(fd[0]);
	return (0);
}
