/*
** FT_SCANF - Implémentation personnalisée de la fonction scanf
**
** Objectif : Parser et extraire des données formatées depuis l'entrée standard
** Principe : Analyse caractère par caractère selon un format donné
*/
#include <ctype.h>  // Pour isspace(), isdigit()
#include <stdarg.h> // Pour la gestion des arguments variables (va_list,
					// va_arg...)
#include <stdio.h>  // Pour FILE*, fgetc(), ungetc()

/*
** match_space - Ignore tous les espaces blancs dans le flux d'entrée
**
** @param f : pointeur vers le flux de fichier
** @return : 0 si succès, -1 si EOF rencontré
**
** Principe : Lit et ignore tous les caractères d'espacement (espaces, tabs,
	newlines...)
** jusqu'à trouver un caractère non-espace ou EOF
*/
int	match_space(FILE *f)
{
	int	c;

	// Lit et ignore tous les espaces blancs
	while (isspace(c = fgetc(f)) && c != EOF)
		;         // Boucle vide : on ignore simplement les espaces
	if (c == EOF) // Si fin de fichier atteinte
		return (-1);
	ungetc(c, f); // Remet le dernier caractère lu (non-espace) dans le flux
	return (0);   // Succès
}

/*
** match_char - Vérifie si le prochain caractère correspond au caractère attendu
**
** @param f : pointeur vers le flux de fichier
** @param c : caractère attendu
** @return : 1 si correspondance, 0 si pas de correspondance, -1 si EOF
**
** Principe : Lit un caractère et vérifie s'il correspond à celui attendu
*/
int	match_char(FILE *f, char c)
{
	int	r;

	r = fgetc(f); // Lit le prochain caractère
	if (r == EOF) // Si fin de fichier
		return (-1);
	if (r == c) // Si le caractère correspond
		return (1);
	else
	{
		ungetc(r, f); // Remet le caractère dans le flux (il ne correspond pas)
		return (0);   // Pas de correspondance
	}
}

/*
** scan_char - Lit un seul caractère depuis le flux d'entrée
**
** @param f : pointeur vers le flux de fichier
** @param ap : liste des arguments variables (contient un char*)
** @return : 1 si succès, -1 si EOF
**
** Principe : Lit exactement un caractère et le stocke dans la variable pointée
*/
int	scan_char(FILE *f, va_list ap)
{
	char	*ptr;

	// Pointeur vers la variable où stocker le caractère
	int c;                    // Caractère lu
	ptr = va_arg(ap, char *); // Récupère le pointeur depuis les arguments
	c = fgetc(f);             // Lit un caractère
	if (c != EOF)             // Si caractère valide lu
	{
		*ptr = c;   // Stocke le caractère dans la variable pointée
		return (1); // Succès : 1 conversion réalisée
	}
	return (-1); // Échec : EOF rencontré
}

/*
** scan_int - Lit un nombre entier depuis le flux d'entrée
**
** @param f : pointeur vers le flux de fichier
** @param ap : liste des arguments variables (contient un int*)
** @return : 1 si succès, 0 si aucun chiffre, -1 si EOF
**
** ALGORITHME :
** 1. Gestion du signe (optionnel + ou -)
** 2. Lecture des chiffres et construction du nombre
** 3. Application du signe et stockage du résultat
*/
int	scan_int(FILE *f, va_list ap)
{
	int c;     // Caractère lu
	int sign;  // Signe du nombre (1 ou -1)
	int i;     // Valeur numérique accumulée
	int digit; // Compteur de chiffres lus
	int *ptr;  // Pointeur vers la variable où stocker l'entier
	// Initialisation
	sign = 1;
	i = 0;
	digit = 0;
	ptr = va_arg(ap, int *); // Récupère le pointeur depuis les arguments
	// Étape 1 : Gestion du signe (optionnel)
	if ((c = fgetc(f)) == '-')
		sign = -1; // Nombre négatif
	else if (c == '+')
	{
		sign = 1; // Nombre positif (explicite)
	}
	else
	{
		if (c == EOF) // Fin de fichier immédiate
			return (-1);
		ungetc(c, f); // Remet le caractère (ce n'est pas un signe)
	}
	// Étape 2 : Lecture et construction du nombre
	while (isdigit(c = fgetc(f)) && c != EOF)
	{
		i = i * 10 + (c - 48); // Conversion ASCII vers chiffre (48 = '0')
		digit++;               // Compte les chiffres lus
	}
	// Vérification : au moins un chiffre doit avoir été lu
	if (!digit)
		return (0); // Aucune conversion possible
	// Remet le dernier caractère non-chiffre dans le flux
	if (c != EOF)
		ungetc(c, f);
	// Étape 3 : Stockage du résultat final
	*ptr = i * sign; // Applique le signe et stocke
	return (1);      // Succès : 1 conversion réalisée
}

/*
** scan_string - Lit une chaîne de caractères (mot) depuis le flux d'entrée
**
** @param f : pointeur vers le flux de fichier
** @param ap : liste des arguments variables (contient un char*)
** @return : 1 si succès, 0 si aucun caractère non-espace lu
**
** Principe : Lit tous les caractères non-espaces consécutifs pour former un "mot"
** S'arrête au premier espace, tab, newline ou EOF
*/
int	scan_string(FILE *f, va_list ap)
{
	int readed; // Nombre de caractères lus
	int i;      // Index pour construire la chaîne
	char *ptr;  // Pointeur vers le buffer de destination
	int c;      // Caractère lu
	readed = 0;
	i = 0;
	ptr = va_arg(ap, char *); // Récupère le pointeur depuis les arguments
	// Lit tous les caractères non-espaces
	while ((c = fgetc(f)) != EOF && !isspace(c))
	{
		ptr[i++] = c; // Stocke le caractère
		readed++;     // Compte les caractères lus
	}
	// Vérification : au moins un caractère doit avoir été lu
	if (!readed)
		return (0); // Aucune conversion possible
	// Remet le dernier caractère (espace ou EOF) dans le flux
	if (c != EOF)
		ungetc(c, f);
	ptr[i] = '\0'; // Terminateur de chaîne
	return (1);    // Succès : 1 conversion réalisée
}

/*
** match_conv - Dispatcher principal : gère les différents types de conversion
**
** @param f : pointeur vers le flux de fichier
** @param format : pointeur vers le caractère de format (c, d, s...)
** @param ap : liste des arguments variables
** @return : résultat de la fonction de scan correspondante
**
** Principe : Selon le caractère de format,
	appelle la fonction de scan appropriée
** Certains formats nécessitent d'ignorer les espaces avant la lecture
*/
int	match_conv(FILE *f, const char **format, va_list ap)
{
	switch (**format) // Selon le caractère de format
	{
	case 'c': // Caractère : pas d'espaces à ignorer
		return (scan_char(f, ap));
	case 'd': // Entier : ignore les espaces avant
		match_space(f);
		return (scan_int(f, ap));
	case 's': // Chaîne : ignore les espaces avant
		match_space(f);
		return (scan_string(f, ap));
	case EOF: // Fin de format inattendue
		return (-1);
	default: // Format non reconnu
		return (-1);
	}
}

/*
** ft_vfscanf - Fonction cœur : parse le format et orchestre les conversions
**
** @param f : pointeur vers le flux de fichier à lire
** @param format : chaîne de format (ex: "%d %s %c")
** @param ap : liste des arguments variables
** @return : nombre de conversions réussies, ou EOF en cas d'erreur
**
** ALGORITHME PRINCIPAL :
** 1. Parcourt la chaîne de format caractère par caractère
** 2. Si '%' → lance une conversion selon le caractère suivant
** 3. Si espace → ignore les espaces dans l'entrée
** 4. Sinon → vérifie la correspondance littérale du caractère
*/
int	ft_vfscanf(FILE *f, const char *format, va_list ap)
{
	int nconv; // Nombre de conversions réussies
	int c;     // Caractère de test pour EOF
	nconv = 0;
	// Test initial : vérifier qu'il y a des données à lire
	c = fgetc(f);
	if (c == EOF)
		return (EOF);
	ungetc(c, f); // Remet le caractère pour la suite
	// Boucle principale : parcours de la chaîne de format
	while (*format)
	{
		if (*format == '%') // Début d'une spécification de conversion
		{
			format++; // Passe au caractère de format (d, s, c...)
			if (match_conv(f, &format, ap) != 1)
				break ; // Échec de conversion → arrêt
			else
				nconv++; // Conversion réussie → compteur++
		}
		else if (isspace(*format)) // Espace dans le format
		{
			if (match_space(f) == -1) // Ignore les espaces dans l'entrée
				break ;                // EOF rencontré → arrêt
		}
		else if (match_char(f, *format) != 1) // Caractère littéral
			break ;                            // Pas de correspondance → arrêt
		format++;
		// Passe au caractère suivant du format
	}
	// Vérification d'erreur de flux
	if (ferror(f))
		return (EOF);
	return (nconv); // Retourne le nombre de conversions réussies
}

/*
** ft_scanf - Interface publique : wrapper autour de ft_vfscanf pour stdin
**
** @param format : chaîne de format (ex: "%d %s")
** @param ... : arguments variables (pointeurs vers les variables à remplir)
** @return : nombre de conversions réussies
**
** Principe : Configure les arguments variables et appelle ft_vfscanf sur stdin
** C'est l'équivalent de scanf() de la libc
*/
int	ft_scanf(const char *format, ...)
{
	va_list ap;                              // Liste des arguments variables
	va_start(ap, format);                    // Initialise la liste
	int ret = ft_vfscanf(stdin, format, ap); // Appel de la fonction principale
	va_end(ap);                              // Nettoie la liste
	return (ret);                            // Retourne le résultat
}

int	main(void)
{
	int		age;
	char	name[50] = {0};
	char	grade;
	int		result;

	printf("=== TESTS SIMPLES DE FT_SCANF ===\n\n");
	// TEST 1: Un seul entier
	printf("1. Entrez votre âge: ");
	result = ft_scanf("%d", &age);
	printf("   Résultat: %d conversion, âge = %d\n", result, age);
	printf("2. Entrez votre nom: ");
	result = ft_scanf("%s", name);
	printf("   Résultat: %d conversion, nom = '%s'\n", result, name);
	printf("3. Entrez âge et nom ensemble: ");
	result = ft_scanf("%d %s", &age, name);
	printf("Adresse de age: %p\n", &age);
	printf("Adresse de name: %p\n", name);
	printf("   Résultat: %d conversions, âge = %d, nom = '%s'\n", result, age,
		name);
	// TEST 2: Une chaîne
	// printf("2. Entrez votre nom: ");
	// result = ft_scanf("%s", name);
	// printf("   Résultat: %d conversion, nom = '%s'\n\n", result, name);
	// // TEST 3: Un caractère
	// printf("3. Entrez une note (A-F): ");
	// result = ft_scanf("%c", &grade);
	// printf("   Résultat: %d conversion, note = '%c'\n\n", result, grade);
	// // TEST 4: Plusieurs valeurs
	// printf("4. Entrez âge et nom séparés: ");
	// result = ft_scanf("%d %s", &age, name);
	// printf("   Résultat: %d conversions, âge = %d, nom = '%s'\n\n", result,
	// age,
	// 	name);
	return (0);
}
