#include <stdio.h>
#include <stdlib.h>

static int	board[20];

void	print_solution(int queens)
{
	int	i;

	i = 0;
	while (i < queens)
	{
		fprintf(stdout, "%d", board[i]);
		if (i + 1 != queens)
			fprintf(stdout, " ");
		i++;
	}
	fprintf(stdout, "\n");
}

int	check_placement(int queens, int row, int col)
{
	int	i;

	i = 0;
	while (i < col)
	{
		if (board[i] == row || (board[i] + col == row + i) || (i
				+ board[i] == row + col))
			return (0);
		i++;
	}
	return (1);
}

void	n_queens(int queens, int col)
{
	int	row;

	row = 0;
	if (col == queens)
	{
		print_solution(queens);
		return ;
	}
	while (row < queens)
	{
		if (check_placement(queens, row, col))
		{
			board[col] = row;
			n_queens(queens, col + 1);
		}
		row++;
	}
}

int	main(int ac, char **av)
{
	if (ac == 2)
	{
		if (atoi(av[1]) <= 2)
		{
			fprintf(stdout, "\n");
			return (0);
		}
		n_queens(atoi(av[1]), 0);
		return (0);
	}
	return (1);
}