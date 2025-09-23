#include <stdio.h>

int	is_unbalanced(char *str)
{
	int	opened;
	int	closed;
	int	i;

	opened = 0;
	closed = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '(')
			opened++;
		else if (str[i] == ')')
		{
			if (opened > 0)
				opened--;
			else
				closed++;
		}
		i++;
	}
	return (opened + closed);
}

void	rip(char *str, int m_fix, int n_fix, int pos)
{
	int		i;
	char	c;

	i = pos;
	if (m_fix == n_fix && !is_unbalanced(str))
	{
		puts(str);
		return ;
	}
	while (str[i])
	{
		if (str[i] == '(' || str[i] == ')')
		{
			c = str[i];
			str[i] = '_';
			rip(str, m_fix + 1, n_fix, i);
			str[i] = c;
		}
		i++;
	}
}

int	main(int ac, char **av)
{
	if (ac == 2)
	{
		rip(av[1], 0, is_unbalanced(av[1]), 0);
	}
}