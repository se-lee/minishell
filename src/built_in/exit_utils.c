#include "minishell.h"

int	strisnum(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

void	remove_space(char **str)
{
	char	*temp;

	temp = ft_strtrim(*str, " ");
	free(*str);
	*str = temp;
}

int	value_exceeds_llint(char *str, long long int return_value)
{
	char	*temp;

	temp = ft_itoa(return_value);
	if (ft_strcmp(str, temp) != 0)
	{
		free(temp);
		return (TRUE);
	}
	else
	{
		free(temp);
		return (FALSE);
	}
}

int	exit_simple(int return_value)
{
	printf("exit\n");
	return (return_value);
}
