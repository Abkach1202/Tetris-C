#ifndef MODELE_H
#define MODELE_H

#include <stdint.h>

// Macro servant de marge dans le terrain du jeu
#define BASE 1
// Macro pour la taille d'une forme (Toutes les formes ont 4 cases)
#define NB_CASES_FORME 4


// dépendance entre la forme et le modèle
typedef struct forme Forme;

// Enumération des couleurs dans le jeu
typedef enum couleur { ROUGE = 1, VERT, JAUNE, BLEU, MAGENTA, CYAN, BLANC, NOIR } Couleur;

// Structure d'un couple d'entier
typedef struct couple {
  int16_t x, y;
} Couple;

// Structure du modèle du jeu Tetris
typedef struct modele {
  uint16_t nbLignes, nbColonnes, score, delai, coef;
  Forme *forme, *suivante;
  Couleur *terrain;
} Modele;

/**
 * @brief Crée et initialise le modèle du jeu tetris.
 * @param nbLignes représente le nombre de ligne du terrain du jeu.
 * @param nbColonnes représente le nombre de colonnes du terrain du jeu.
 * @return le modèle crée (que l'on doit liberer) ou NULL si il y'a erreur.
 */
Modele *initModele(uint16_t nbLignes, uint16_t nbColonnes);

/**
 * @brief Détruit et libère l'espace occupée par le modèle du jeu.
 * @param modele représente le modèle à détruire.
 */
void detruitModele(Modele *modele);

/**
 * @brief Permet d'avoir le score du jeu.
 * @param modele représente le modèle du jeu.
 * @return le score du joueur dans la partie.
 */
uint16_t getScore(Modele *modele);

/**
 * @brief Permet d'avoir le délai d'attente avant chaque itération. Cela nous permet de controler la
 * rapidité du jeu
 * @param modele représente le modèle du jeu.
 * @return le délai d'attente avant chaque itération.
 */
uint16_t getDelai(Modele *modele);

/**
 * @brief Permet d'avoir le nombre de lignes du terrain du jeu.
 * @param modele représente le modèle du jeu.
 * @return le nombre ligne du terrain du jeu.
 */
uint16_t getNbLignes(Modele *modele);

/**
 * @brief Permet d'avoir le nombre de colonnes du terrain du jeu.
 * @param modele représente le modèle du jeu.
 * @return le nombre ligne du terrain du jeu.
 */
uint16_t getNbColonnes(Modele *modele);

/**
 * @brief Copie une partie du terrain du modèle dans un tableau de couleur.
 * @param modele représente le modèle du jeu.
 * @param terrain représente un pointeur vers un espace où stocker les couleurs.
 * @param x représente l'abscisse à laquelle il commence à copier.
 * @param y représente l'ordonnées à laquelle il commence à copier.
 * @param w représente la largeur de la partie à copier.
 * @param h représente la hauteur de la partie à copier
 */
void getTerrain(Modele *modele, Couleur *terrain, uint16_t x, uint16_t y, uint16_t w, uint16_t h);

/**
 * @brief Permet d'avoir les coordonnées de la forme courante dans le terrain.
 * @param modele représente le modèle du jeu contenant la forme.
 * @param coords représente un pointeur vers un espace où stocker les coordonnées.
 */
void getCoordFormeCourante(Modele *modele, Couple *coords);

/**
 * @brief Permet d'avoir les coordonnées relatives de la forme suivante.
 * @param modele représente le modèle du jeu contenant la forme.
 * @param coords représente un pointeur vers un espace où stocker les coordonnées.
 */
void getCoordFormeSuivante(Modele *modele, Couple *coords);

/**
 * @brief Permet d'avoir la couleur de la forme courante.
 * @param modele représente le modèle du jeu.
 * @return la couleur de la forme courante sur le terrain.
 */
Couleur getCouleurFormeCourante(Modele *modele);

/**
 * @brief Permet d'avoir la couleur de la forme suivante.
 * @param modele représente le modèle du jeu.
 * @return la couleur de la forme suivante.
 */
Couleur getCouleurFormeSuivante(Modele *modele);

/**
 * @brief Vérifie si la case de coordonnées (x, y) est occupée ou c'est à dire si sa couleur est
 * différente de NOIR.
 * @param modele représente le modèle du jeu.
 * @param x représente le numéro de colonne de la case.
 * @param y représente le numéro de ligne de la case.
 * @return 1 si elle est occupée et 0 si non.
 */
uint8_t estOccupee(Modele *modele, uint16_t x, uint16_t y);

/**
 * @brief Enregistre la forme sur le terrain en recopiant sa couleur sur ses coordonnées
 * dans le terain.
 * @param modele représente le modèle du jeu contenant la forme. (Paramètre modifié)
 */
void deposeForme(Modele *modele);

/**
 * @brief Déplace la forme courante d'une case vers le bas dans le terrain du jeu.
 * @param modele représente le modèle du jeu contenant la forme à avancer. (Paramètre modifié)
 * @return 1 si il y'a eu collision, 0 si non et -1 si il y'a eu erreur en créant la forme suivante
 */
int8_t formeAvance(Modele *modele);

/**
 * @brief Décale la forme courante d'une case vers la gauche dans le terrain du jeu.
 * @param modele représente le modèle du jeu contenant la forme à décaler. (Paramètre modifié)
 */
void formeDecaleGauche(Modele *modele);

/**
 * @brief Décale la forme courante d'une case vers la droite dans le terrain du jeu.
 * @param modele représente le modèle du jeu contenant la forme décaler. (Paramètre modifié)
 */
void formeDecaleDroite(Modele *modele);

/**
 * @brief Tourne la forme courante d'un angle de 90° dans le sens des aiguilles
 * d'une montre dans le terrain du jeu.
 * @param modele représente le modèle du jeu contenant la forme à tourner. (Paramètre modifié)
 */
void formeTourne(Modele *modele);

/**
 * @brief Vérifie si la ligne est complète c'est à dire si toutes les cases de la ligne sont
 * occupée.
 * @param modele représente le modèle du jeu.
 * @param y représente le numéro de la ligne à vérifier.
 * @return 1 si elle est complète et 0 si non.
 */
uint8_t estLigneComplete(Modele *modele, uint16_t y);

/**
 * @brief Supprime la ligne spécifiée en copiant la ligne du dessus pour toutes les lignes se
 * trouvant entre la première et celle spécifiée.
 * @param modele représente le modèle du jeu. (Paramètre modifié)
 * @param y représente le numéro de la ligne à supprimer.
 */
void supprimeLigne(Modele *modele, uint16_t y);

/**
 * @brief Parcours le terrain du jeu et supprime toutes les lignes complètes.
 * @param modele représente le modèle du jeu. (Paramètre modifié)
 */
void supprimeLignesCompletes(Modele *modele);

/**
 * @brief Vérifie si le jeu est terminé c'est à dire si il y'a une forme qui deborde du terrain.
 * @param modele représente le modèle du jeu.
 * @return 1 si il y'a au moins une forme qui deborde et 0 si non.
 */
uint8_t estTermine(Modele *modele);

/**
 * @brief Recommence le jeu en nettoyant le terrain du jeu.
 * @param modele représente le modèle du jeu.
 * @return 0 si tous s'est bien passée et -1 si non.
 */
int8_t recommenceModele(Modele *modele);

#endif