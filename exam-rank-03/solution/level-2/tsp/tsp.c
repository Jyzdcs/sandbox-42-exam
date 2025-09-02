#include <errno.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

// compute the distance between two points
float	distance(float a[2], float b[2])
{
	return (sqrtf((b[0] - a[0]) * (b[0] - a[0]) + (b[1] - a[1]) * (b[1]
				- a[1])));
}

// Variables globales pour le backtracking
static int used[11];                   
	// Marque les villes déjà visitées (0=non visitée, 1=visitée)
static int path[11];                   
	// Chemin en cours de construction (stocke les indices des villes)
static float	best_distance = 999999.0f;
		// Meilleure distance trouvée jusqu'ici

// Fonction récursive qui trouve tous les chemins possibles
static void	find_path(float (*array)[2], int pos, int size,
		float current_distance)
{
	int	i;

	// Cas de base : on a visité toutes les villes
	if (pos == size)
	{
		// Ajoute la distance de retour à la ville de départ
		current_distance += distance(array[path[pos - 1]], array[path[0]]);
		// Met à jour la meilleure distance si on a trouvé mieux
		if (current_distance < best_distance)
			best_distance = current_distance;
		return ;
	}
	// Essaie chaque ville comme prochaine étape
	i = 0;
	while (i < size)
	{
		// Si la ville n'est pas encore visitée
		if (!used[i])
		{
			used[i] = 1;   // Marque la ville comme visitée
			path[pos] = i; // Ajoute la ville au chemin
			// Récursion : continue avec la ville suivante
			if (pos == 0)
				// Première ville : pas de distance à ajouter
				find_path(array, pos + 1, size, 0);
			else
				// Ajoute la distance depuis la ville précédente
				find_path(array, pos + 1, size, current_distance
					+ distance(array[path[pos - 1]], array[i]));
			used[i] = 0; // Backtrack : démarque la ville
		}
		i++;
	}
}

// Fonction principale appelée par le main
float	tsp(float (*array)[2], ssize_t size)
{
	best_distance = 999999.0f;    // Initialise avec une grande valeur
	find_path(array, 0, size, 0); // Lance le backtracking depuis la position 0
	return (best_distance);       // Retourne la meilleure distance trouvée
}

ssize_t	file_size(FILE *file)
{
	char	*buffer;
	size_t	n;
	ssize_t	ret;

	buffer = NULL;
	n = 0;
	errno = 0;
	for (ret = 0; getline(&buffer, &n, file) != -1; ret++)
		;
	free(buffer);
	if (errno || fseek(file, 0, SEEK_SET))
		return (-1);
	return (ret);
}

int	retrieve_file(float (*array)[2], FILE *file)
{
	int	tmp;

	for (size_t i = 0; (tmp = fscanf(file, "%f, %f\n", array[i] + 0, array[i]
				+ 1)) != EOF; i++)
		if (tmp != 2)
		{
			errno = EINVAL;
			return (-1);
		}
	if (ferror(file))
		return (-1);
	return (0);
}

int	main(int ac, char **av)
{
	char *filename = "stdin";
	FILE *file = stdin;
	if (ac > 1)
	{
		filename = av[1];
		file = fopen(filename, "r");
	}

	if (!file)
	{
		fprintf(stderr, "Error opening %s: %m\n", filename);
		return (1);
	}

	ssize_t size = file_size(file);
	if (size == -1)
	{
		fprintf(stderr, "Error reading %s: %m\n", filename);
		fclose(file);
		return (1);
	}

	float(*array)[2] = calloc(size, sizeof(float[2]));
	if (!array)
	{
		fprintf(stderr, "Error: %m\n");
		fclose(file);
		return (1);
	}

	if (retrieve_file(array, file) == -1)
	{
		fprintf(stderr, "Error reading %s: %m\n", av[1]);
		fclose(file);
		free(array);
		return (1);
	}
	if (ac > 1)
		fclose(file);

	printf("%.2f\n", tsp(array, size));
	free(array);
	return (0);
}