/*
** GET_NEXT_LINE - Lecture ligne par ligne d'un fichier
**
** Objectif : Lire un fichier ligne par ligne, une ligne à chaque appel
** Principe : Utiliser un buffer statique pour garder les données entre les appels
*/
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#ifndef BUFFER_SIZE
# define BUFFER_SIZE 1
// Taille du buffer de lecture (peut être changée à la compilation)
#endif

/*
** ft_bzero - Remplit une zone mémoire avec des zéros
**
** @param s : pointeur vers la zone mémoire à initialiser
** @param n : nombre d'octets à mettre à zéro
**
** Fonction utilitaire pour initialiser proprement la mémoire
*/
void	ft_bzero(void *s, size_t n)
{
	while (n--)           // Tant qu'il reste des octets à traiter
		*(char *)s++ = 0; // Met l'octet à 0 et avance le pointeur
}

/*
** ft_strdup - Duplique une chaîne de caractères
**
** @param src : chaîne source à dupliquer
** @return : nouveau pointeur vers la chaîne dupliquée (à libérer avec free)
**
** Étapes :
** 1. Calcule la longueur de la chaîne source
** 2. Alloue la mémoire nécessaire (+1 pour le '\0')
** 3. Copie caractère par caractère
** 4. Ajoute le terminateur de chaîne
*/
char	*ft_strdup(char *src)
{
	char	*dest;
	int		i;

	// Étape 1 : Calcul de la longueur
	i = 0;
	while (src[i])
		i++;
	// Étape 2 : Allocation mémoire (longueur + 1 pour '\0')
	dest = (char *)malloc(sizeof(char) * (i + 1));
	// Étape 3 : Copie caractère par caractère
	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	// Étape 4 : Ajout du terminateur
	dest[i] = '\0';
	return (dest);
}

/*
** get_next_line
	- Fonction principale : lit une ligne depuis un descripteur de fichier
**
** @param fd : descripteur de fichier (file descriptor)
** @return : pointeur vers la ligne lue (à libérer avec free) ou NULL si fin de fichier/erreur
**
** PRINCIPE CLÉ : Utilisation de variables STATIQUES pour garder l'état entre les appels
**
** Variables statiques (persistent entre les appels) :
** - buffer[] : stocke les données lues depuis le fichier
** - buffer_read : nombre de caractères actuellement dans le buffer
** - buffer_pos : position actuelle de lecture dans le buffer
**
** ALGORITHME :
** 1. Vérification des paramètres
** 2. Boucle de construction de ligne :
**    - Si buffer vide → lire depuis le fichier
**    - Copier caractère par caractère jusqu'à trouver '\n' ou EOF
** 3. Retourner la ligne construite
*/
char	*get_next_line(int fd)
{
	static char	buffer[BUFFER_SIZE];
	char		line[70000];
	static int	buffer_read;

	// Buffer statique : garde les données entre appels
	// Buffer temporaire pour construire la ligne
	// Statique : nombre de chars lus dans buffer
	static int buffer_pos; // Statique : position actuelle dans buffer
	int i;                 // Index pour construire la ligne
	i = 0;
	// Étape 1 : Vérifications de sécurité
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	// Étape 2 : Boucle principale de construction de ligne
	while (1)
	{
		// Si on a lu tout le buffer, il faut relire depuis le fichier
		if (buffer_pos >= buffer_read)
		{
			buffer_read = read(fd, buffer, BUFFER_SIZE);
			// Lecture depuis le fichier
			buffer_pos = 0;
			// Reset position dans buffer
			if (buffer_read <= 0)
				// Fin de fichier ou erreur
				break ;
		}
		// Copie du caractère actuel du buffer vers la ligne
		line[i++] = buffer[buffer_pos++];
		// Si on vient de copier un '\n', on a fini la ligne
		if (buffer[buffer_pos] == '\n')
			break ;
	}
	// Étape 3 : Finalisation et retour
	line[i] = '\0'; // Terminateur de chaîne
	if (i == 0)     // Si aucun caractère lu, fin de fichier
		return (NULL);
	return (ft_strdup(line));
	// Duplique la ligne pour la retourner (allocation dynamique)
}

// int	main(void)
// {
// 	int fd;
// 	char *line = NULL;

// 	fd = open("test.txt", O_RDONLY);
// 	while (line = get_next_line(fd))
// 	{
// 		printf("%s\n", line);
// 		free(line);
// 	}
// 	free(line);
// 	return (0);
// }