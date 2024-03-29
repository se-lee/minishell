/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 07:42:34 by acusanno          #+#    #+#             */
/*   Updated: 2021/11/26 07:42:36 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_heredoc(t_vars *vars)
{
	t_redirect	*current_in;
	int			heredoc_count;

	current_in = vars->inout;
	heredoc_count = 0;
	while (current_in)
	{
		if (current_in->arrow_num == 2)
			heredoc_count++;
		current_in = current_in->next;
	}
	return (heredoc_count);
}

char	*get_delimiter(t_redirect *current_inout)
{
	char	*delimiter;

	if (current_inout->filename == NULL)
	{
		perror("");
		return (NULL);
	}
	else
	{
		delimiter = ft_strdup(current_inout->filename);
		return (delimiter);
	}
}

void	display_and_put_lines(char *line, int fd)
{
	ft_putstr_fd("> ", OUT);
	ft_putstr_fd(line, fd);
	ft_putstr_fd("\n", fd);
	free(line);
}

void	write_to_heredoc(t_redirect *current_in)
{
	int		fd;
	char	*delimiter;
	char	*line;

	delimiter = get_delimiter(current_in);
	fd = open(".heredoc", O_TRUNC | O_CREAT | O_WRONLY, 0644);
	if (fd < 0)
		perror("heredoc_file");
	line = NULL;
	ft_putstr_fd("> ", OUT);
	while (get_next_line(IN, &line))
	{
		if (ft_strcmp(line, delimiter))
			display_and_put_lines(line, fd);
		else
		{
			free(line);
			break ;
		}
	}
	close(fd);
	free(delimiter);
}

void	update_heredoc(t_vars *vars)
{
	t_redirect	*current_inout;

	current_inout = vars->inout;
	while (current_inout && current_inout->side == REDIRECT_LEFT)
	{
		if (current_inout->arrow_num == 2)
			write_to_heredoc(current_inout);
		current_inout = current_inout->next;
	}
}
