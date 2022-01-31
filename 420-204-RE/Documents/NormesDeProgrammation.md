# Normes de programmation

## Nomenclature

- Variables : nomDeVariable
- Données membres : donnéeMembre_
- Constante : CONSTANTE
- Méthodes, Fonctions et Classes : CamelCase
- Fichiers : NomDeLaClasse + *.hpp || *.cpp

## Garde de redéclaration :

Pour chque fichier d'en-tête :

```cpp
#ifndef NOMDUFICHIER_HPP
#define NOMDUFICHIER_HPP

// Déclaration et Implémentation

#endif
```              

## Esthétisme

- Tabulations de 4 espaces.
- Mettre des espaces dans une opération mathématique.

```cpp
x = 3 + 3;
```

- Code en anglais et documentation en français.
- On utilise using namespace pour éviter de préciser les namespace.
- On organise notre code de la façon la plus visuelle possible.

*Exemple, lorsqu'on déclare une matrice, on aligne les lignes et les colonnes.*

- On utilise le mot clé inline.
- Les if et les else qui contiennent une seule ligne de code ne doivent pas avoir de brackets.
- Il faut mettre des espaces avant et après les () :

```cpp
if ()
    // TODO : Code
```

- Il faut laisser une ligne entre chaque fonctions :

```cpp
void Void() {
    // TODO : Code
}
```
