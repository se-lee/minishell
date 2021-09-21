#include "minishell.h"

int	isspecial(char c)
{
	if ((c == '"') || (c == '\'') || (c == '$')
	|| (c == '<') || (c == '>') || (c == '|'))
		return (1);
	else
		return (0);
}

void	ft_comandadd_back(t_command **alst, t_command *new)
{
	t_command	*ptr;

	ptr = *alst;
	if (!(*alst))
		*alst = new;
	else
	{
		while (ptr->next != NULL)
			ptr = ptr->next;
		ptr->next = new;
		new->next = NULL;
	}
}
