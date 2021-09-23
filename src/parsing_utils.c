#include "minishell.h"

int	isspecial(char c)
{
	if ((c == '"') || (c == '\'') || (c == ' ')
	|| (c == '<') || (c == '>') || (c == '|'))
		return (1);
	else
		return (0);
}

int	ft_isupper(char c)
{
	if (c >= 'A' && c <= 'Z')
		return (1);
	return(0);
}

void	*protected_malloc(size_t count, size_t size)
{
	void	*p;
	size_t	i;

	i = 0;
	p = malloc(size * count);
	if (!p)
	{
		perror("Malloc failed\n");
		exit(-1);
	}
	;
	return (p);
}

void	ft_comandadd_back(t_token **alst, t_token *new)
{
	t_token	*ptr;

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
