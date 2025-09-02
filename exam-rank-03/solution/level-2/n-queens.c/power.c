#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int	power(int base, int exp)
{
	if (exp == 0)
		return (1);
	return (base * power(base, exp - 1));
}

int	main(int ac, char **av)
{
	int i = atoi(av[1]);
	int j = atoi(av[2]);
	printf("%d\n", power(i, j));
	return (0);
}