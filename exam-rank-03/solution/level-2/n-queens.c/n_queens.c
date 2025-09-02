#include <stdio.h>
#include <stdlib.h>

static int	board[20];

static void	print_solution(int n)
{
	int	i;

	for (i = 0; i < n; i++)
	{
		fprintf(stdout, "%d", board[i]);
		if (i < n - 1)
			fprintf(stdout, " ");
	}
	fprintf(stdout, "\n");
}

static int	ft_abs(int n)
{
	if (n < 0)
		return (-n);
	return (n);
}

static int	is_safe(int row, int col)
{
	int	i;

	i = 0;
	while (i < col)
	{
		if (board[i] == row || ft_abs(board[i] - row) == ft_abs(i - col))
			return (0);
		i++;
	}
	return (1);
}

static void	solve(int col, int n)
{
	int	row;

	if (col >= n)
	{
		print_solution(n);
		return ;
	}
	row = 0;
	while (row < n)
	{
		if (is_safe(row, col))
		{
			board[col] = row;
			solve(col + 1, n);
		}
		row++;
	}
}

int	main(int ac, char **av)
{
	int n;

	if (ac != 2)
		return (1);
	n = atoi(av[1]);
	if (n <= 0 || n > 20)
		return (1);
	solve(0, n);
	return (0);
}