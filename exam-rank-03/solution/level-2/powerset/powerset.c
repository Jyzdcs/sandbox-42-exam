#include <stdio.h>
#include <stdlib.h>

static int	*numbers;
static int	*current;
static int	count;
static int	target;

static void	print_subset(int size)
{
	int	i;

	for (i = 0; i < size; i++)
	{
		printf("%d", current[i]);
		if (i < size - 1)
			printf(" ");
	}
	printf("\n");
}

static void	find_subsets(int pos, int sum, int size)
{
	int	i;

	if (sum == target && size > 0)
		print_subset(size);
	if (pos >= count)
		return ;
	i = pos;
	while (i < count)
	{
		if (sum + numbers[i] <= target)
		{
			current[size] = numbers[i];
			find_subsets(i + 1, sum + numbers[i], size + 1);
		}
		i++;
	}
}

int	main(int ac, char **av)
{
	int i;

	if (ac < 3)
		return (1);

	target = atoi(av[1]);
	count = ac - 2;

	numbers = malloc(sizeof(int) * count);
	current = malloc(sizeof(int) * count);
	if (!numbers || !current)
	{
		free(numbers);
		free(current);
		return (1);
	}

	i = 0;
	while (i < count)
	{
		numbers[i] = atoi(av[i + 2]);
		i++;
	}

	find_subsets(0, 0, 0);

	free(numbers);
	free(current);
	return (0);
}