#include "minishell.h"

/*
command->redirect_left == 1 : input
command->redirect_left == 2 : heredoc

command->redirect_right == 1 : output overwrite
command->redirect_right == 2 : output append

$ < test.txt cat : this should display the file
heredoc <<

$ > test.txt: this should write nothing (empty file)

*/

int	redirect_input(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		perror(file);
	if (dup2(fd, STDIN_FILENO) == -1)
		perror("dup2");
	close(fd);
	return (0);
}

int		redirect_heredoc(t_vars *vars);

int	redirect_output_overwrite(char *file)
{
	int	fd;

	fd = open(file, O_TRUNC | O_CREAT | O_WRONLY, 0644);
	if (fd < 0)
		perror(file);
	if (dup2(fd, STDOUT_FILENO) == -1)
		perror("dup2");
	close(fd);
	return (0);
}

int	redirect_output_append(char *file)
{
	int	fd;
	fd = open(file, O_APPEND | O_CREAT | O_WRONLY, 0644);
	if (fd < 0)
		perror(file);
	if (dup2(fd, STDOUT_FILENO) == -1)
		perror ("dup2");
	close (fd);
	return (0);
}

void	redirection(t_vars *vars, t_command *current_cmd)
{
	t_redirect	*current_in;
	t_redirect	*current_out;

printf("current_cmd_left:%d\n", current_cmd->redirect_left);
printf("current_cmd_right:%d\n", current_cmd->redirect_right);
	if (current_cmd->redirect_left == 1)
		redirect_input(current_in->filename);
	else if (current_cmd->redirect_right == 1)
		redirect_output_overwrite(current_out->filename);
	else if (current_cmd->redirect_right == 2)
		redirect_output_append(current_out->filename);
	current_in = current_in->next;
	current_out = current_out->next;
}
