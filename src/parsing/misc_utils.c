#include "minishell.h"

void	ft_append(char **a, char *str)
{
	char	*result;

	result = ft_strjoin(*a, str);
	free(*a);
	*a = result;
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
	return (p);
}

void	free_array(char **array)
{
    int    i;

    i = 0;
    while (array[i])
    {
        free(array[i]);
        i++;
    }
    free(array);
}
