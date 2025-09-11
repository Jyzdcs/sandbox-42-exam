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
		fprintf(stdout, " ");
		i++;
	}
	fprintf(stdout, "\n");
}

int	valide_pos(int queens, int row)
{
	int	i;

	i = 0;
	while (i < row)
	{
		if ((board[i] == board[row]) || (row - board[row] == i - board[i])
			|| (row + board[row] == i + board[i]))
			return (0);
		i++;
	}
	return (1);
}

void	solve(int queens, int row)
{
	int	i;

	if (row == queens)
	{
		print_solution(queens);
		return ;
	}
	i = 0;
	while (i < queens)
	{
		board[row] = i;
		if (valide_pos(queens, row))
			solve(queens, row + 1);
		i++;
	}
}

int	main(int ac, char **av)
{
	if (ac == 2)
	{
		if (atoi(av[1]) <= 2)
		{
			ftprintf(stdout, "\n");
			return (0);
		}
		solve(atoi(av[1]), 0);
	}
	return (0);
}