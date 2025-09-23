#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int	strcmpa(char *s1, char *s2, int len)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] == s1[i] && i < len)
		i++;
	if (len == i)
		return (0);
	return (1);
}

void	filter(char *s, char *filter, int len)
{
	int		i;
	int		y;
	char	buffer;

	i = 0;
	while (read(0, &buffer, 1))
	{
		s[i++] = buffer;
		if (i >= 10000)
			break ;
	}
	i = 0;
	while (s[i])
	{
		if (strcmpa(&s[i], filter, len) == 0 && i + len < 10000)
		{
			y = 0;
			while (y < len)
			{
				printf("*");
				y++;
			}
			i += y;
		}
		else
		{
			printf("%c", s[i]);
			i++;
		}
	}
}

int	main(int ac, char **av)
{
	if (ac == 2)
	{
		char *input = av[1];
		int i = 0;
		char *s = malloc(sizeof(char) * 10001);
		if (!s)
			return (1);
		while (input[i])
			i++;
		filter(s, av[1], i);
		free(s);
	}
}