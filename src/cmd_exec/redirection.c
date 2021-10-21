#include "minishell.h"

/*
command->redirect_left == 1 : input
command->redirect_left == 2 : heredoc

command->redirect_right == 1 : output overwrite
command->redirect_right == 2 : output append



$ < test.txt cat : this should display the file

amend >>
heredoc <<

*/

// if command has < (REDIRET_LEFT)
int	redirect_input(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		perror((char *)file);
	if (dup2(fd, STDIN_FILENO) == -1)
		perror("dup2");
	close(fd);
	return (0);
}

// the case where < input comes first
int	

// > (REDIRECT_RIGHT)
int	redirect_output_overwrite(char *file)
{
	int	fd;

	fd = open(file, O_TRUNC | O_CREAT | O_WRONLY, 0644);
	if (fd < 0)
		perror((char *)file);
	if (dup2(fd, STDOUT_FILENO) == -1)
		perror("dup2");
	close(fd);
	return (0);
}

int	redirect_output_append(char *file);

