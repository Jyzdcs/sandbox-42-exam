#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void	swap(char *s1, char *s2)
{
	char	tmp;

	tmp = *s1;
	*s1 = *s2;
	*s2 = tmp;
}

void	sort(char *str)
{
	int	i;
	int	j;

	i = 0;
	while (str[i])
	{
		j = i + 1;
		while (str[j])
		{
			if (str[i] > str[j])
				swap(&str[i], &str[j]);
			j++;
		}
		i++;
	}
}

void	permutations(char *str, int len, int start)
{
	int	i;

	i = start;
	if (start == len)
	{
		puts(str);
		return ;
	}
	while (i < len)
	{
		swap(&str[start], &str[i]);
		permutations(str, len, start + 1);
		swap(&str[start], &str[i]);
		i++;
	}
}

int	main(int ac, char **av)
{
	if (ac == 2)
	{
		int len = 0;
		char *solution;
		while (av[1][len])
			len++;
		solution = malloc(sizeof(char) + (len + 1));
		if (!solution)
			return (1);
		sort(av[1]);
		permutations(av[1], len, 0);
		free(solution);
	}
}