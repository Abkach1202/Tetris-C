#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "forme.h"

// Énumération de toutes les formes dans une variable globale
static const Couple LES_FORMES[7][4] = {
    {{-1, 1}, {-1, 0}, {0, 0}, {1, 0}}, {{-1, 0}, {0, 0}, {0, 1}, {1, 1}},
    {{-1, 1}, {0, 1}, {0, 0}, {1, 0}},  {{-1, 0}, {0, 0}, {1, 0}, {1, 1}},
    {{-1, 1}, {0, 1}, {0, 0}, {1, 1}},  {{0, 0}, {1, 0}, {1, 1}, {0, 1}},
    {{0, 0}, {0, 1}, {0, 2}, {0, 3}}};

/**
 * @brief Implementation de la fonction initForme.
 */
Forme *initForme(Modele *modele) {
  // Création de la forme et gestion d'erreur
  Forme *forme = (Forme *)malloc(sizeof(Forme));
  if (!forme) {
    perror("Erreur à la création de la forme : Allocation mémoire échouée");
    return NULL;
  }
  // Initialisation de la couleur
  forme->couleur = 1 + rand() % 7;
  // Initialisation des coordonnées d'origine
  forme->x0 = rand() % (modele->nbColonnes - 2) + 1;
  forme->y0 = 0;
  // Initialisation du modele
  forme->modele = modele;

  // Initialisation de la forme choisie et gestion d'erreur
  forme->forme = (Couple *)malloc(NB_CASES_FORME * sizeof(Couple));
  if (!forme->forme) {
    free(forme);
    perror("Erreur à la création de la forme : Allocation mémoire échouée");
    return NULL;
  }
  memcpy(forme->forme, LES_FORMES[rand() % 7], NB_CASES_FORME * sizeof(Couple));
  return forme;
}

/**
 * @brief Implémentation de la fonction detruitForme.
 */
void detruitForme(Forme *forme) {
  if (!forme)
    return;
  // Destruction des coordonnées
  free(forme->forme);
  // La forme ne detruit pas le modèle
  free(forme);
}

/**
 * @brief Implémentation de la fonction getCouleur.
 */
Couleur getCouleur(Forme *forme) {
  return forme->couleur;
}

/**
 * @brief Implémentation de la fonction getCoordonnees.
 */
void getCoordonees(Forme *forme, Couple *coords) {
  memcpy(coords, forme->forme, NB_CASES_FORME * sizeof(Couple));
}

/**
 * @brief Implémentation de la fonction getCoordonneesTerrain.
 */
void getCoordoneesTerrain(Forme *forme, Couple *coords) {
  for (int i = 0; i < NB_CASES_FORME; i++)
    coords[i] = (Couple){forme->forme[i].x + forme->x0, forme->forme[i].y + forme->y0};
}

/**
 * @brief Implémentation de la fonction estEnCollision.
 */
uint8_t estEnCollision(Forme *forme) {
  Couple c;
  // On parcours les coordonnées
  for (int i = 0; i < NB_CASES_FORME; i++) {
    c = (Couple){forme->forme[i].x + forme->x0, forme->forme[i].y + forme->y0};
    // On vérifie si la case en dessous deborde ou est occupée
    if (c.y == getNbLignes(forme->modele) - 1 || estOccupee(forme->modele, c.x, c.y + 1))
      return 1;
  }
  return 0;
}

/**
 * @brief Implémentation de la fonction coordonneesValides
 */
uint8_t coordonneesValides(Forme *forme) {
  Couple c;
  // On parcours les coordonnées
  for (int i = 0; i < NB_CASES_FORME; i++) {
    c = (Couple){forme->forme[i].x + forme->x0, forme->forme[i].y + forme->y0};
    // On vérifie si elles sont valides
    if (!(0 <= c.y && c.y < getNbLignes(forme->modele) && 0 <= c.x &&
          c.x < getNbColonnes(forme->modele) && !estOccupee(forme->modele, c.x, c.y)))
      return 0;
  }
  return 1;
}

/**
 * @brief Implémentation de la fonction avance.
 */
void avance(Forme *forme) {
  forme->y0 += 1;
}

/**
 * @brief Implémentation de la fonction decaleGauche.
 */
void decaleGauche(Forme *forme) {
  forme->x0 -= 1;
  if (!coordonneesValides(forme))
    forme->x0 += 1;
}

/**
 * @brief Implémentation de la fonction decaleDroite.
 */
void decaleDroite(Forme *forme) {
  forme->x0 += 1;
  if (!coordonneesValides(forme))
    forme->x0 -= 1;
}

/**
 * @brief Implémentation de la fonction tourne.
 */
void tourne(Forme *forme) {
  Couple forme_tmp[NB_CASES_FORME];
  memcpy(forme_tmp, forme->forme, NB_CASES_FORME * sizeof(Couple));
  for (int i = 0; i < NB_CASES_FORME; i++)
    forme->forme[i] = (Couple){-1 * forme_tmp[i].y, forme_tmp[i].x};
  if (!coordonneesValides(forme))
    memcpy(forme->forme, forme_tmp, NB_CASES_FORME * sizeof(Couple));
}