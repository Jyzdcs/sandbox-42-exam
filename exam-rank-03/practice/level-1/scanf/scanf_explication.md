# FT_SCANF - Parser d'entrée formatée

## 🎯 Objectif

Recréer la fonction `scanf()` qui parse l'entrée utilisateur selon un format donné (ex: `"%d %s %c"`). C'est un excellent exercice de **parsing** et de **gestion d'arguments variables**.

## 🧠 Concepts clés

### 1. Arguments variables (`va_list`)

```c
int ft_scanf(const char *format, ...)  // "..." = nombre variable d'args
{
    va_list ap;                    // Liste des arguments
    va_start(ap, format);          // Initialise la liste
    char *ptr = va_arg(ap, char*); // Récupère un argument
    va_end(ap);                    // Nettoie
}
```

### 2. Gestion des flux (`FILE*`)

```c
int c = fgetc(f);     // Lit un caractère
ungetc(c, f);         // Remet le caractère dans le flux
```

## 📋 Architecture modulaire

### Vue d'ensemble

```
ft_scanf()
    ↓
ft_vfscanf()  ← Cœur du parsing
    ↓
match_conv()  ← Dispatcher selon format
    ↓
scan_int() / scan_char() / scan_string()  ← Fonctions spécialisées
```

## 🔧 Fonctions par fonction

### 1. `match_space()` - Ignore les espaces

```c
Entrée: "   42"
       ↑ ignore ces espaces
Résultat: position sur '4'
```

### 2. `match_char()` - Vérifie un caractère exact

```c
Format: "Age: %d"
Entrée: "Age: 25"
        ↑ vérifie que c'est bien 'A', puis 'g', puis 'e', puis ':'
```

### 3. `scan_int()` - Parse un entier

```c
Entrée: "-42abc"
Étapes:
  1. Lit '-' → sign = -1
  2. Lit '4' → i = 4
  3. Lit '2' → i = 4*10 + 2 = 42
  4. Lit 'a' → pas un chiffre, remet dans flux
  5. Résultat: -42
```

### 4. `scan_string()` - Parse une chaîne

```c
Entrée: "Hello World"
Résultat: "Hello" (s'arrête à l'espace)
Position: sur ' ' (espace)
```

### 5. `scan_char()` - Lit exactement un caractère

```c
Entrée: "A42"
Résultat: 'A'
Position: sur '4'
```

## 🎬 Trace d'exécution complète

**Appel:** `ft_scanf("%d %s %c", &age, name, &grade)`  
**Entrée:** `"25 Alice A"`

### Phase 1: Parse "%d"

```
1. match_conv() voit 'd' → appelle scan_int()
2. match_space() ignore les espaces
3. scan_int() lit "25" → stocke dans &age
4. nconv = 1
```

### Phase 2: Parse " " (espace dans format)

```
1. match_space() ignore les espaces dans l'entrée
2. Position: sur 'A' de "Alice"
```

### Phase 3: Parse "%s"

```
1. match_conv() voit 's' → appelle scan_string()
2. match_space() (déjà fait)
3. scan_string() lit "Alice" → stocke dans name
4. nconv = 2
```

### Phase 4: Parse " " + "%c"

```
1. match_space() ignore l'espace
2. scan_char() lit 'A' → stocke dans &grade
3. nconv = 3
```

**Résultat:** `return 3` (3 conversions réussies)

## 🛠️ Points techniques importants

### Gestion des erreurs

```c
if (match_conv() != 1)
    break;  // Arrêt immédiat si échec
```

### Remise en flux (`ungetc`)

```c
// Si on lit un caractère qui ne nous intéresse pas:
ungetc(c, f);  // Le remet pour la prochaine lecture
```

### Comptage des conversions

```c
nconv++;  // Seulement si conversion réussie
return nconv;  // Nombre total de conversions
```

## 💡 Stratégie de reproduction

### 1. **Analyse du format**

- Parcourir caractère par caractère
- Distinguer: `%d`, espaces, caractères littéraux

### 2. **Dispatcher modulaire**

- Une fonction par type de conversion
- Centraliser dans `match_conv()`

### 3. **Gestion du flux**

- Toujours pouvoir "annuler" une lecture avec `ungetc()`
- Gérer les espaces intelligemment

### 4. **Arguments variables**

- Maîtriser `va_list`, `va_arg()`, etc.
- Récupérer les pointeurs correctly

### 5. **Tests systématiques**

- Tester chaque format individuellement
- Combiner les formats
- Gérer les cas d'erreur

## 🧪 Cas de test utiles

```c
// Test basique
ft_scanf("%d", &n);  // Entrée: "42"

// Test combiné
ft_scanf("%d %s", &age, name);  // Entrée: "25 John"

// Test avec caractères littéraux
ft_scanf("Age: %d", &age);  // Entrée: "Age: 30"

// Test d'erreur
ft_scanf("%d", &n);  // Entrée: "abc" → return 0
```

Cette approche modulaire peut s'adapter à d'autres formats (`%f`, `%x`, etc.) en ajoutant simplement de nouvelles fonctions de scan !
