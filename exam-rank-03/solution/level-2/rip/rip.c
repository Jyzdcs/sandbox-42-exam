#include <stdio.h>

static char	str[1000];
static char	result[1000];
static int	len;
static int	min_removed;

static int	check(char *s)
{
	int	count;

	count = 0;
	while (*s)
	{
		if (*s == '(')
			count++;
		else if (*s == ')')
		{
			count--;
			if (count < 0)
				return (0);
		}
		s++;
	}
	return (count == 0);
}

static void	generate(int pos, int removed)
{
	if (pos == len)
	{
		if (check(result) && removed <= min_removed)
		{
			if (removed < min_removed)
				min_removed = removed;
			if (removed == min_removed)
				puts(result);
		}
		return ;
	}
	result[pos] = str[pos];
	generate(pos + 1, removed);
	if (str[pos] == '(' || str[pos] == ')')
	{
		result[pos] = ' ';
		generate(pos + 1, removed + 1);
	}
}

int	main(int ac, char **av)
{
	if (ac != 2)
		return (1);

	len = 0;
	while (av[1][len])
		str[len] = av[1][len++];
	str[len] = '\0';

	min_removed = len;
	generate(0, 0);

	return (0);
}