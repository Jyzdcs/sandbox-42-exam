# GET_NEXT_LINE - Lecture ligne par ligne

## 🎯 Objectif

Implémenter une fonction qui lit un fichier **ligne par ligne**, une ligne à chaque appel. C'est un défi classique de gestion mémoire et de parsing.

## 🧠 Concept clé : Les variables statiques

La **difficulté principale** est de garder l'état entre les appels successifs de la fonction. C'est ici qu'interviennent les **variables statiques**.

```c
static char buffer[BUFFER_SIZE];  // Gardé entre les appels
static int buffer_read;           // Combien de chars dans le buffer
static int buffer_pos;            // Position actuelle dans le buffer
```

### Pourquoi statique ?

- **Variable normale** : détruite à la fin de la fonction ❌
- **Variable statique** : gardée en mémoire entre les appels ✅

## 📋 Algorithme principal

### 1. Structure de données

```
Fichier: "Hello\nWorld\nTest"
         ↓
Buffer statique: ['H','e','l','l','o']  (BUFFER_SIZE = 5)
                  ↑
              buffer_pos = 0
```

### 2. Logique de lecture

```c
while (1) {
    // Si buffer vide → lire depuis le fichier
    if (buffer_pos >= buffer_read) {
        buffer_read = read(fd, buffer, BUFFER_SIZE);
        buffer_pos = 0;
    }

    // Copier caractère par caractère
    line[i++] = buffer[buffer_pos++];

    // Si on trouve '\n' → ligne complète !
    if (buffer[buffer_pos] == '\n')
        break;
}
```

## 🔍 Trace d'exécution

Fichier: `"Hi\nBye"` avec `BUFFER_SIZE = 3`

**Appel 1:**

```
read() → buffer = ['H','i','\n']
Copie 'H', 'i'
Trouve '\n' → retourne "Hi"
État: buffer_pos = 3, buffer_read = 3
```

**Appel 2:**

```
buffer_pos >= buffer_read → read() → buffer = ['B','y','e']
Copie 'B', 'y', 'e'
EOF → retourne "Bye"
```

**Appel 3:**

```
read() retourne 0 (EOF)
Aucun caractère → retourne NULL
```

## 🛠️ Fonctions utilitaires

### `ft_strdup`

- **Rôle** : Duplique la ligne construite
- **Pourquoi** : La ligne locale est détruite à la fin de la fonction
- **Résultat** : Allocation dynamique (à libérer avec `free()`)

### `ft_bzero`

- **Rôle** : Initialise proprement la mémoire
- **Usage** : Éviter les données résiduelles

## 💡 Points clés à retenir

1. **Variables statiques** = persistance entre appels
2. **Buffer** = optimisation des lectures systèmes
3. **Position tracking** = où on en est dans le buffer
4. **Allocation dynamique** = pour retourner la ligne
5. **Gestion EOF** = détecter la fin de fichier

## 🚀 Comment reproduire cette approche

1. **Identifier le problème** : besoin de garder un état
2. **Choisir les variables statiques** : buffer + positions
3. **Créer la boucle de lecture** : remplir buffer → construire ligne
4. **Gérer les cas limites** : EOF, erreurs, lignes vides
5. **Optimiser** : taille de buffer, allocation mémoire

Cette approche peut s'adapter à d'autres problèmes nécessitant de garder un état entre appels de fonction !
