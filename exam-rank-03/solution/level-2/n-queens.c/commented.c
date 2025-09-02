#include <stdio.h>
#include <stdlib.h>

static int	queen[20]; // queen[column] = line where queen is

void	print_solutions(int *queen, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		fprintf(stdout, "%d", queen[i]);
		fprintf(stdout, " ");
		i++;
	}
	fprintf(stdout, "\n");
}

int	is_safe(int *queen, int column, int row)
{
	int	i;

	i = 0;
	while (i < column)
	{
		if (queen[i] == row || queen[i] - i == row - column || queen[i]
			+ i == row + column)
			// va check si la pos de la queen est sur la meme ligne,
			// ainsi que sur les deux diagonales
			return (0);
		// si c'est le cas, c'est pas safe
		i++;
	}
	return (1); // safe
}

void	solve(int column, int n, int *queen)
{
	int	row;

	if (column == n)
	{
		// une solution found, on l'imprime, et on continue (return)
		// pour que la recursivite s'occupe des autres solutions potentielles
		print_solutions(queen, n);
		return ;
	}
	row = 0;
	while (row < n) // traverse toutes les lignes
	{
		if (is_safe(queen, column, row))
		// si la position ou on tente de poser la queen est safe
		{
			queen[column] = row;
			// alors on met une queen en column x sera a ligne y
			solve(column + 1, n, queen);
			// on cherche des solutions pour les autres columns
		}
		row++;
	}
}

int	main(int ac, char **av)
{
	if (ac == 2)
	{
		int n = atoi(av[1]); // total number of queens to place
		if (n <= 2)
		{
			fprintf(stdout, "\n");
			return (0);
		}
		solve(0, n, queen); // start column 0 and do it i times
	}
	return (0);
}