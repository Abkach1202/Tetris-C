#include <stdio.h>
#include <stdlib.h>

#include "vueNcurses.h"

// Macro pour la marge de colonne entre deux boxs
#define MARGE_COL 6
// Macro pour la marge de ligne entre deux boxs
#define MARGE_LIG 3
// Macro pour la largeur d'une case
#define LARG_CASE 4
// Macro pour la hauteur d'une case
#define HAUT_CASE 2
// Macro pour la dimension du terrain de la forme suivante
#define DIM 6
// macro pour le code ASCII de la touche échap
#define ESCAPE 27

/**
 * @brief Implémentation de la fonction creeBox.
 */
WINDOW *creeBox(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
  WINDOW *win = subwin(stdscr, h + 2, w + 2, y - 1, x - 1);
  box(win, 0, 0);
  return win;
}

/**
 * @brief Implémentation de la fonction creeLesCases.
 */
WINDOW **creeLesCases(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
  WINDOW **wins = (WINDOW **)malloc(h * w * sizeof(WINDOW *));
  int i, j;
  for (i = 0; i < h; i++) {
    for (j = 0; j < w; j++) {
      wins[i * w + j] = subwin(stdscr, HAUT_CASE, LARG_CASE, y + i * HAUT_CASE, x + j * LARG_CASE);
      box(wins[i * w + j], 0, 0);
      wbkgd(wins[i * w + j], COLOR_PAIR(NOIR));
    }
  }
  return wins;
}

/**
 * @brief Implémentation de la fonction initVueNcurses.
 */
Vue *initVueNcurses(uint16_t nbLignes, uint16_t nbColonnes) {
  uint16_t x, y, w, h;
  // Création de la vue Ncurses
  Vue *vue = (Vue *)malloc(sizeof(Vue));
  if (!vue) {
    perror("Erreur à la création de la vue Ncurses : Allocation mémoire échouée");
    return NULL;
  }
  vue->nbLignes = nbLignes;
  vue->nbColonnes = nbColonnes;

  // Création des données de la vue Ncurses
  VueNcurses *data = (VueNcurses *)malloc(sizeof(VueNcurses));
  if (!data) {
    perror("Erreur à la création de la vue Ncurses : Allocation mémoire échouée");
    free(vue);
    return NULL;
  }
  vue->data = data;

  // Initialisation de Ncurses
  initscr();
  noecho();               // pour ne pas afficher les clics de l'utilisateur
  nodelay(stdscr, TRUE);  // Pour que getch ne bloque pas le programme
  keypad(stdscr, TRUE);   // Pour activer les directions
  curs_set(0);            // Pour ne pas voir le curseur

  // Initialisation des couleurs
  if (has_colors() == FALSE) {
    fprintf(stderr, "Votre terminal ne supporte pas les couleurs : Arrêt du programme\n");
    free(vue);
    free(data);
    endwin();
    return NULL;
  }
  start_color();
  init_pair(BLANC, COLOR_BLACK, COLOR_WHITE);
  init_pair(NOIR, COLOR_WHITE, COLOR_BLACK);
  init_pair(ROUGE, COLOR_WHITE, COLOR_RED);
  init_pair(VERT, COLOR_WHITE, COLOR_GREEN);
  init_pair(JAUNE, COLOR_WHITE, COLOR_YELLOW);
  init_pair(BLEU, COLOR_WHITE, COLOR_BLUE);
  init_pair(MAGENTA, COLOR_WHITE, COLOR_MAGENTA);
  init_pair(CYAN, COLOR_WHITE, COLOR_CYAN);

  // Création de la box du jeu
  w = nbColonnes * LARG_CASE + 3 * MARGE_COL + DIM * LARG_CASE;
  h = nbLignes * HAUT_CASE + 2 * MARGE_LIG;
  x = (COLS - w) / 2;
  y = (LINES - h) / 2;
  data->boxJeu = creeBox(x, y, w, h);

  // Création de la box et des cases du terrain
  x = x + MARGE_COL;
  y = y + MARGE_LIG;
  data->boxTerrain = creeBox(x, y, nbColonnes * LARG_CASE, nbLignes * HAUT_CASE);
  data->terrain = creeLesCases(x, y, nbColonnes, nbLignes);
  // Affichage du label du jeu
  mvprintw(y, x - 1 + nbColonnes * LARG_CASE + (2 * MARGE_COL + DIM * LARG_CASE - 12) / 2,
           "T E T R I S");

  // Création de la box et des cases de la forme suivante
  w = DIM * LARG_CASE;
  h = DIM * HAUT_CASE;
  x = x + nbColonnes * LARG_CASE + MARGE_COL;
  y = (LINES - h) / 2;
  data->boxSuivante = creeBox(x, y, w, h);
  data->suivante = creeLesCases(x, y, DIM, DIM);
  // Affichage du label de la forme suivante
  mvprintw(y - 2, x - 1 + (w - 16) / 2, "S U I V A N T E");

  // Création de la box et du label du score du jeu
  y = y + DIM * HAUT_CASE + MARGE_LIG;
  data->boxScore = creeBox(x, y, DIM * LARG_CASE, 1);

  // Création de la box pour des éventuels messages
  y = y + MARGE_LIG;
  data->boxMessage = subwin(stdscr, 4 * HAUT_CASE + 2, DIM * LARG_CASE + 2, y - 1, x - 1);

  // Rafraîchissement
  refresh();

  // Initialisation du reste des variables
  vue->metVueAJour = metVueAJourNcurses;
  vue->ecoute = ecouteNcurses;
  vue->detruitVue = detruitVueNcurses;
  return vue;
}

/**
 * @brief Implémentation de la fonction detruitVueNcurses.
 */
void detruitVueNcurses(Vue *vue) {
  if (!vue)
    return;
  VueNcurses *data = (VueNcurses *)vue->data;
  int i, j;
  // Destruction de la box du jeu
  delwin(data->boxJeu);
  // Destruction de la box du terrain
  delwin(data->boxTerrain);
  // Destruction de la box du terrain de la forme suivante
  delwin(data->boxSuivante);
  // Destruction de la box du score
  delwin(data->boxScore);
  // Destruction de la box des messages
  delwin(data->boxMessage);
  // Destruction de toutes les cases du terrain du jeu
  for (i = 0; i < vue->nbLignes; i++)
    for (j = 0; j < vue->nbColonnes; j++)
      delwin(data->terrain[i * vue->nbColonnes + j]);
  // Destruction de toutes les cases du terrain de la forme suivante
  for (i = 0; i < DIM; i++)
    for (j = 0; j < DIM; j++)
      delwin(data->suivante[i * DIM + j]);
  // Clôture de Ncurses
  endwin();
  // Libération des mémoires
  free(data->terrain);
  free(data->suivante);
  free(data);
  free(vue);
}

/**
 * @brief Implémentation de la fonction dessineCaseNcurses.
 */
void dessineCaseNcurses(Vue *vue, uint16_t x, uint16_t y, Couleur couleur) {
  VueNcurses *data = (VueNcurses *)vue->data;
  wbkgd(data->terrain[y * vue->nbColonnes + x], COLOR_PAIR(couleur));
}

/**
 * @brief Implémentation de la fonction dessineCaseSuivanteNcurses.
 */
void dessineCaseSuivanteNcurses(Vue *vue, uint16_t x, uint16_t y, Couleur couleur) {
  VueNcurses *data = (VueNcurses *)vue->data;
  wbkgd(data->suivante[y * DIM + x], COLOR_PAIR(couleur));
}

/**
 * @brief Implémentation de la fonction dessineFormeNcurses.
 */
void dessineFormeNcurses(Vue *vue, Couple *coords, Couleur couleur) {
  // On parcours les coordonées
  for (int i = 0; i < NB_CASES_FORME; i++)
    // On vérifie si la forme ne deborde pas sur la base
    if (0 <= coords[i].y - BASE)
      dessineCaseNcurses(vue, coords[i].x, coords[i].y - BASE, couleur);
}

/**
 * @brief Implémentation de la fonction dessineFormeSuivanteNcurses.
 */
void dessineFormeSuivanteNcurses(Vue *vue, Couple *coords, Couleur couleur) {
  // Nettoie le terrain de la forme suivante
  nettoieTerrainSuivanteNcurses(vue);
  // On parcours les coordonées et on dessine
  for (int i = 0; i < NB_CASES_FORME; i++)
    dessineCaseSuivanteNcurses(vue, coords[i].x + 2, coords[i].y + 2, couleur);
}

/**
 * @brief Implémentation de la fonction dessineTerrainNcurses.
 */
void dessineTerrainNcurses(Vue *vue, Couleur *terrain) {
  int i, j;
  // On parcours et on dessine
  for (i = 0; i < vue->nbLignes; i++)
    for (j = 0; j < vue->nbColonnes; j++)
      dessineCaseNcurses(vue, j, i, terrain[i * vue->nbColonnes + j]);
}

/**
 * @brief Implémentation de la fonction nettoieTerrainSuivanteNcurses.
 */
void nettoieTerrainSuivanteNcurses(Vue *vue) {
  int i, j;
  // On parcours et on nettoie
  for (i = 0; i < DIM; i++)
    for (j = 0; j < DIM; j++)
      dessineCaseSuivanteNcurses(vue, j, i, NOIR);
}

/**
 * @brief Implémentation de la fonction afficherScoreNcurses.
 */
void afficheScoreNcurses(Vue *vue, uint16_t score) {
  VueNcurses *data = (VueNcurses *)vue->data;
  mvwprintw(data->boxScore, 1, (DIM * LARG_CASE - 18) / 2, "S C O R E : %hu", score);
}

/**
 * @brief Implémentation de la fonction afficherScoreNcurses.
 */
void afficheMessageNcurses(Vue *vue, char *s) {
  VueNcurses *data = (VueNcurses *)vue->data;
  werase(data->boxMessage);
  mvwprintw(data->boxMessage, 1, 0, "%s", s);
  flushinp();
}

/**
 * @brief Implémentation de la fonction ecouteNcurses.
 */
Evenement ecouteNcurses() {
  int c = getch();
  switch (c) {
    case KEY_UP :
      return FHAUT;
    case KEY_DOWN :
      return FBAS;
    case KEY_LEFT :
      return FGAUCHE;
    case KEY_RIGHT :
      return FDROITE;
    case ' ' :
      return ESPACE;
    case KEY_ENTER :
    case '\n' :
      return ENTREE;
    case 'R' :
    case 'r' :
      return TOUCHE_R;
    case ESCAPE :
      return ECHAP;
    default :
      return RIEN;
  }
}

/**
 * @brief Implémentation de la fonction afficherScoreNcurses.
 */
void rafraichiVueNcurses(Vue *vue) {
  VueNcurses *data = (VueNcurses *)vue->data;
  int i, j;
  // rafraichissement des messages
  wrefresh(data->boxScore);
  wrefresh(data->boxMessage);
  // rafraichissement de toutes les cases du terrain du jeu
  for (i = 0; i < vue->nbLignes; i++)
    for (j = 0; j < vue->nbColonnes; j++)
      wrefresh(data->terrain[i * vue->nbColonnes + j]);
  // rafraichissement de toutes les cases du terrain de la forme suivante
  for (i = 0; i < DIM; i++)
    for (j = 0; j < DIM; j++)
      wrefresh(data->suivante[i * DIM + j]);
}

/**
 * @brief Implémentation de la fonction metVueAjourNcurses.
 */
uint8_t metVueAJourNcurses(Vue *vue,
                           Modele *modele,
                           int8_t errEtColl,
                           uint16_t pause,
                           uint16_t fini) {
  if (errEtColl == -1)
    return 1;
  Couple coords[NB_CASES_FORME];
  Couleur terrain[vue->nbLignes * vue->nbColonnes];

  // Si il y'a collision, on dessine la forme suivante
  if (errEtColl) {
    getCoordFormeSuivante(modele, coords);
    dessineFormeSuivanteNcurses(vue, coords, getCouleurFormeSuivante(modele));
  }
  // On dessine le terrain
  getTerrain(modele, terrain, 0, BASE, vue->nbColonnes, vue->nbLignes);
  dessineTerrainNcurses(vue, terrain);
  // On dessine la forme
  getCoordFormeCourante(modele, coords);
  dessineFormeNcurses(vue, coords, getCouleurFormeCourante(modele));
  // On met à jour le score
  afficheScoreNcurses(vue, getScore(modele));

  // On met le message à jour
  if (fini) {
    afficheMessageNcurses(vue, MSG_FIN);
  } else if (pause) {
    afficheMessageNcurses(vue, MSG_PAUSE);
  } else {
    afficheMessageNcurses(vue, MSG_JEU);
  }
  rafraichiVueNcurses(vue);
  return 0;
}
