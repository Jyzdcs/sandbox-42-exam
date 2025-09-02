#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void	swap(char *a, char *b)
{
	char	temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

void	sort(char *s)
{
	int		i;
	int		j;
	char	tmp;

	i = 0;
	while (s[i])
	{
		j = i + 1;
		while (s[j])
		{
			if (s[i] > s[j])
				swap(&s[i], &s[j]);
			j++;
		}
		i++;
	}
}
void	permute_swap(char *s, int start, int len)
{
	int	i;

	if (start == len)
	{
		puts(s);
		return ;
	}
	i = start;
	while (i < len)
	{
		// Échange s[start] avec s[i]
		swap(&s[start], &s[i]);
		// Récursion sur la partie restante
		permute_swap(s, start + 1, len);
		// Backtrack : échange à nouveau pour restaurer
		swap(&s[start], &s[i]);
		i++;
	}
}

int	main(int ac, char **av)
{
	if (ac == 2)
	{
		char *s = av[1];
		int i = 0;
		while (s[i])
			i++;
		char *solution = malloc(i + 1);
		solution[i] = '\0';
		sort(s);
		permute_swap(s, 0, i);
		free(solution);
	}
	return (0);
}