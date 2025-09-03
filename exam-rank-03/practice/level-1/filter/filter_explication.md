# FILTER - Filtre de texte avec remplacement

## 🎯 Objectif

Créer un programme qui lit depuis **stdin** et remplace toutes les occurrences d'un mot par des **étoiles**. C'est l'équivalent de `sed 's/mot/****/g'` mais implémenté from scratch.

## 📋 Cahier des charges

### Utilisation

```bash
./filter <mot_à_remplacer>
```

### Exemples

```bash
echo 'abcdefgaaaabcdefabc' | ./filter abc
# Sortie: ***defgaaa***def***

echo 'Hello world Hello' | ./filter Hello
# Sortie: ***** world *****
```

### Contraintes

- **1 seul argument** (le mot à remplacer)
- Argument **non vide**
- Lecture depuis **stdin** uniquement
- **Fonctions autorisées** : `read`, `strlen`, `malloc`, `calloc`, `realloc`, `free`, `printf`, `perror`

## 🧠 Défis techniques

### 1. **Pas de strstr() autorisée**

→ Il faut réimplémenter la recherche de sous-chaîne

### 2. **Traitement en temps réel**

→ Pas de stockage complet en mémoire, traitement au fur et à mesure

### 3. **Gestion des occurrences multiples**

→ Scanner et remplacer toutes les occurrences dans chaque bloc

## 🔧 Architecture de la solution

### Vue d'ensemble

```
main()
  ↓ (lit par blocs)
filter_line()
  ↓ (cherche occurrences)
find_substring()
  ↓ (compare caractère par caractère)
```

## 📝 Algorithmes détaillés

### `find_substring()` - Recherche de sous-chaîne

```c
Texte: "abcdefabc"
Search: "abc"

Position 0: 'a' vs 'a' ✓, 'b' vs 'b' ✓, 'c' vs 'c' ✓ → TROUVÉ!
Retourne: pointeur vers position 0
```

**Algorithme:**

1. Pour chaque position dans le texte
2. Compare caractère par caractère avec la recherche
3. Si correspondance complète → retourne la position
4. Sinon → essaie la position suivante

### `filter_line()` - Traitement d'une ligne

```c
Entrée: "abcdefabc"
Search: "abc"

Étape 1: find_substring() → trouve à position 0
Étape 2: affiche "" (rien avant), puis "***"
Étape 3: pos = position 3, continue la recherche
Étape 4: find_substring() → trouve à position 6
Étape 5: affiche "def", puis "***"
Étape 6: pos = position 9 (fin), affiche ""
```

**Résultat:** `***def***`

## 🎬 Trace d'exécution complète

**Commande:** `echo 'Hello Hello world' | ./filter Hello`

### Phase 1: Validation

```c
ac = 2, av[1] = "Hello" → Validation OK
search_len = 5
```

### Phase 2: Lecture

```c
read(0, buffer, 4095) → "Hello Hello world\n"
bytes = 18
buffer[18] = '\0'
```

### Phase 3: Filtrage

```c
filter_line(buffer, "Hello", 5):

1ère occurrence (pos=0):
  - found = 0
  - affiche "" (pos < found = false)
  - affiche "*****" (5 étoiles)
  - pos = 5

2ème occurrence (pos=5):
  - found = 6 (après l'espace)
  - affiche " " (pos=5 < found=6)
  - affiche "*****"
  - pos = 11

Plus d'occurrences:
  - affiche " world\n" (reste)
```

**Sortie finale:** `***** ***** world`

## 💡 Points techniques importants

### 1. **Lecture par blocs**

```c
while ((bytes = read(0, buffer, 4095)) > 0)
```

- **Pourquoi 4095 et pas 4096 ?** → Réserver 1 octet pour `'\0'`
- **Optimisation** : lecture efficace vs lecture caractère par caractère

### 2. **Gestion mémoire**

- Pas d'allocation dynamique nécessaire ici
- Buffer statique de taille fixe suffit

### 3. **Gestion des erreurs**

```c
if (bytes < 0) {
    perror("Error: ");  // Format exact requis
    return (1);
}
```

### 4. **Performance**

- `strlen(av[1])` calculé **une seule fois**
- Recherche optimisée avec comparaison directe

## 🛠️ Stratégie de reproduction

### 1. **Analyse du problème**

- Identifier : lecture stdin → transformation → sortie stdout
- Contraintes : remplacer toutes occurrences, pas de strstr

### 2. **Décomposition modulaire**

```c
// 1. Fonction de recherche (remplacement strstr)
char *find_substring(char *text, char *search);

// 2. Fonction de traitement d'une ligne
void filter_line(char *line, char *search, int len);

// 3. Main: orchestration générale
int main(int ac, char **av);
```

### 3. **Implémentation étape par étape**

1. **find_substring()** → tester avec exemples simples
2. **filter_line()** → tester cas à 0, 1, multiple occurrences
3. **main()** → ajouter validation + gestion erreurs
4. **Tests** → comparer avec `sed`

### 4. **Tests critiques**

```bash
# Test basique
echo 'abc' | ./filter abc
# Attendu: ***

# Test multiple
echo 'abcabc' | ./filter abc
# Attendu: ******

# Test pas d'occurrence
echo 'xyz' | ./filter abc
# Attendu: xyz

# Test argument vide
./filter ""
# Attendu: exit code 1
```

## 🔍 Variantes possibles

Cette approche peut s'adapter à d'autres transformations :

- **Remplacement par un autre mot** (au lieu d'étoiles)
- **Case-insensitive matching**
- **Expression régulière simple**
- **Comptage d'occurrences**

L'idée clé : **scanner → détecter → transformer → afficher** reste la même !
