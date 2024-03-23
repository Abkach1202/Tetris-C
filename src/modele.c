#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "forme.h"
#include "modele.h"

// Macro pour le maximum du delai d'attente
#define DELAI_MAX 225
// Macro pour le minimum du delai d'attente
#define DELAI_MIN 45
// Macro pour le coefficient d'ajout en fonction du niveau
#define COEF_DELAI 15

/**
 * @brief Implémentation de la fonction initModele.
 */
Modele *initModele(uint16_t nbLignes, uint16_t nbColonnes) {
  // Création du modèle
  Modele *modele = (Modele *)calloc(1, sizeof(Modele));
  if (!modele) {
    perror("Erreur à la création du modèle : Allocation mémoire échouée");
    return NULL;
  }
  // Initialisation du coefficient
  modele->coef = 1;
  // Initialisation du délai
  modele->delai = DELAI_MAX;
  // Initialisation du nombre de lignes
  modele->nbLignes = nbLignes + BASE;
  /// Initilisation du nombre de colonnes
  modele->nbColonnes = nbColonnes;
  // Initialisation de la forme courante et gestion d'erreur
  modele->forme = initForme(modele);
  if (!modele->forme) {
    free(modele);
    return NULL;
  }
  // Initialisation de la forme suivante et gestion d'erreur
  modele->suivante = initForme(modele);
  if (!modele->suivante) {
    detruitForme(modele->forme);
    free(modele);
    return NULL;
  }
  // Création du terrain et gestion d'erreur
  modele->terrain = (Couleur *)malloc(modele->nbLignes * modele->nbColonnes * sizeof(Couleur));
  if (!modele->terrain) {
    perror("Erreur à la création du terrain : Allocation mémoire échouée");
    detruitForme(modele->forme);
    detruitForme(modele->suivante);
    free(modele);
    return NULL;
  }
  // Initialisation du terrain
  int i, j;
  for (i = 0; i < modele->nbLignes; i++)
    for (j = 0; j < modele->nbColonnes; j++)
      modele->terrain[i * modele->nbColonnes + j] = NOIR;
  return modele;
}

/**
 * @brief Implémentation de la fonction detruitModele.
 */
void detruitModele(Modele *modele) {
  if (!modele)
    return;
  // Destruction du terrain
  if (modele->terrain)
    free(modele->terrain);
  // Destruction de la forme courante
  detruitForme(modele->forme);
  // Destruction de la forme suivante
  detruitForme(modele->suivante);
  // Destruction du modèle
  free(modele);
}

/**
 * @brief Implémentation de la fonction getScore.
 */
uint16_t getScore(Modele *modele) {
  return modele->score;
}

/**
 * @brief Implémentation de la fonction getDelai.
 */
uint16_t getDelai(Modele *modele) {
  return modele->delai;
}

/**
 * @brief Implémentation de la fonction getNbLignes.
 */
uint16_t getNbLignes(Modele *modele) {
  return modele->nbLignes;
}

/**
 * @brief Implémentation de la fonction getNbColonnes.
 */
uint16_t getNbColonnes(Modele *modele) {
  return modele->nbColonnes;
}

/**
 * @brief Implémentation de la fonction getTerrain.
 */
void getTerrain(Modele *modele, Couleur *terrain, uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
  int i, j;
  for (i = 0; i < h; i++)
    for (j = 0; j < w; j++)
      terrain[i * w + j] = modele->terrain[(i + y) * modele->nbColonnes + j + x];
}

/**
 * @brief Implémentation de la fonction getCoordFormeCourante.
 */
void getCoordFormeCourante(Modele *modele, Couple *coords) {
  getCoordoneesTerrain(modele->forme, coords);
}

/**
 * @brief Implémentation de la fonction getCoordFormeSuivante.
 */
void getCoordFormeSuivante(Modele *modele, Couple *coords) {
  getCoordonees(modele->suivante, coords);
}

/**
 * @brief Implémentation de la fonction getCouleurFormeCourante.
 */
Couleur getCouleurFormeCourante(Modele *modele) {
  return getCouleur(modele->forme);
}

/**
 * @brief Implémentation de la fonction getCouleurFormeSuivante.
 */
Couleur getCouleurFormeSuivante(Modele *modele) {
  return getCouleur(modele->suivante);
}

/**
 * @brief Implémentation de la fonction estOccupee.
 */
uint8_t estOccupee(Modele *modele, uint16_t x, uint16_t y) {
  return modele->terrain[y * modele->nbColonnes + x] != NOIR;
}

/**
 * @brief Implémentation de la fonction deposeForme.
 */
void deposeForme(Modele *modele) {
  // Récupération des coordonnées de la forme
  Couple coords[NB_CASES_FORME];
  getCoordFormeCourante(modele, coords);
  // Récupération de la couleur
  Couleur couleur = getCouleurFormeCourante(modele);
  // Parcours et ajout
  for (int i = 0; i < NB_CASES_FORME; i++)
    modele->terrain[coords[i].y * modele->nbColonnes + coords[i].x] = couleur;
}

/**
 * @brief Implémentation de la fonction formeAvance.
 */
int8_t formeAvance(Modele *modele) {
  // Si il y'a collision
  if (estEnCollision(modele->forme)) {
    // On dépose la forme courante
    deposeForme(modele);
    // On détruit la forme courante
    detruitForme(modele->forme);
    // On supprime les lignes complètes
    supprimeLignesCompletes(modele);
    // On affecte la suivante à la courante
    modele->forme = modele->suivante;
    // On initialise une nouvelle à la suivante
    modele->suivante = initForme(modele);
    if (!modele->suivante)
      return -1;
    return 1;
  }
  // Si non on fait avancer
  avance(modele->forme);
  return 0;
}

/**
 * @brief Implémentation de la fonction formeDecaleGauche.
 */
void formeDecaleGauche(Modele *modele) {
  decaleGauche(modele->forme);
}

/**
 * @brief Implémentation de la fonction formeDecaleDroite.
 */
void formeDecaleDroite(Modele *modele) {
  decaleDroite(modele->forme);
}

/**
 * @brief Implémentation de la fonction formeTourne.
 */
void formeTourne(Modele *modele) {
  tourne(modele->forme);
}

/**
 * @brief Implémentation de la fonction estLigneComplete.
 */
uint8_t estLigneComplete(Modele *modele, uint16_t y) {
  // On parcours et on retourne 0 si une case n'est pas occupée
  for (int i = 0; i < modele->nbColonnes; i++)
    if (!estOccupee(modele, i, y))
      return 0;
  return 1;
}

/**
 * @brief Implémentation de la fonction supprimeLigne
 */
void supprimeLigne(Modele *modele, uint16_t y) {
  int i, j;
  // On parcours les lignes à partir de la ligne y
  for (i = y; i >= 0; i--)
    for (j = 0; j < modele->nbColonnes; j++) {
      // Si c'est la première ligne, on met du Noir
      if (i == 0)
        modele->terrain[j] = NOIR;
      // Si non on met les couleurs du dessus
      else
        modele->terrain[i * modele->nbColonnes + j] =
            modele->terrain[(i - 1) * modele->nbColonnes + j];
    }
}

/**
 * @brief Implémentation de la fonction supprimeLignesCompletes.
 */
void supprimeLignesCompletes(Modele *modele) {
  // On parcours les lignes
  for (int i = BASE; i < modele->nbLignes; i++)
    // Si la ligne est complete, on la supprime et on ajoute le score
    if (estLigneComplete(modele, i)) {
      supprimeLigne(modele, i);
      modele->score += modele->coef;
    }
  // On met à jour le délai et le coefficient d'ajout
  if (modele->delai > DELAI_MIN) {
    modele->delai -= COEF_DELAI * ((modele->score / 10) + 1 - modele->coef);
    modele->coef = (modele->score / 10) + 1;
  }
}

/**
 * @brief Implémentation de la fonction estTerminee.
 */
uint8_t estTermine(Modele *modele) {
  // On parcours la base
  for (int i = 0; i < modele->nbColonnes; i++)
    // Si il y'a une case occupée, c'est fini
    if (estOccupee(modele, i, 0))
      return 1;
  return 0;
}

/**
 * @brief Implémentation de la fonction recommenceModele.
 */
int8_t recommenceModele(Modele *modele) {
  int i, j;
  // On parcours pour nettoyer le terrain
  for (i = 0; i < modele->nbLignes; i++)
    for (j = 0; j < modele->nbColonnes; j++)
      modele->terrain[i * modele->nbColonnes + j] = NOIR;
  // On detruit les anciennes formes
  detruitForme(modele->forme);
  detruitForme(modele->suivante);
  // On choisit des nouvelles formes
  modele->forme = initForme(modele);
  if (!modele->forme)
    return -1;
  modele->suivante = initForme(modele);
  if (!modele->suivante)
    return -1;
  // On reinitialise le delai et le score
  modele->delai = DELAI_MAX;
  modele->score = 0;
  return 0;
}