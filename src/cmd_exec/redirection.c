#include "minishell.h"

// if command has < 

int	redirect_input(const char *file)
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

//if command has > 
int	redirect_output(const char *file)
{
	int	fd;

	fd = open(file, O_CREAT | O_WRONLY, 0644);
	if (fd < 0)
		perror((char *)file);
	if (dup2(fd, STDOUT_FILENO) == -1)
		perror("dup2");
	close(fd);
	return (0);
}
