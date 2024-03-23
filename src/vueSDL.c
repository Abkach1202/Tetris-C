#include <stdio.h>
#include <stdlib.h>

#include "vue.h"
#include "vueSDL.h"

// Macro pour la largeur de la fenêtre
#define FEN_LARG 1920
// Macro pour la hauteur de la fenêtre
#define FEN_HAUT 1080
// Macro pour la dimanesion d'une case
#define DIM_CASE 35
// Macro pour la dimension du terrain de la suivante
#define DIM 6
// Macro pour la marge en colonne
#define MARGE_COL 60
// Macro pour la marge en ligne
#define MARGE_LIG 50

/**
 * @brief Implémentation de la fonction initVueSDL.
 */
Vue *initVueSDL(uint16_t nbLignes, uint16_t nbColonnes) {
  uint16_t x, y;
  // Création de la vue
  Vue *vue = (Vue *)malloc(sizeof(Vue));
  if (!vue) {
    perror("Erreur à la création de la vue SDL : Allocation mémoire échouée");
    return NULL;
  }
  vue->nbLignes = nbLignes;
  vue->nbColonnes = nbColonnes;

  // Création des données de la vue
  VueSDL *data = (VueSDL *)malloc(sizeof(VueSDL));
  if (!data) {
    perror("Erreur à la création de la vue SDL : Allocation mémoire échouée");
    detruitVueSDL(vue);
    return NULL;
  }
  vue->data = data;

  // Initialisation de la SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "Erreur à l'initialisation de SDL : %s\n", SDL_GetError());
    detruitVueSDL(vue);
    return NULL;
  }

  // Initialisation de TTF pour les textes
  if (TTF_Init() < 0) {
    fprintf(stderr, "Erreur à l'initialisation de SDL_ttf : %s\n", TTF_GetError());
    detruitVueSDL(vue);
    return NULL;
  }

  // Création de la fenêtre du jeu
  data->fenetre = SDL_CreateWindow("Tetris", 0, 0, FEN_LARG, FEN_HAUT, SDL_WINDOW_SHOWN);
  if (!data->fenetre) {
    fprintf(stderr, "Erreur à la création de la fenêtre SDL : %s\n", SDL_GetError());
    detruitVueSDL(vue);
    return NULL;
  }

  // Création d'un rendu pour l'affichage su jeu
  data->renderer = SDL_CreateRenderer(data->fenetre, -1, SDL_RENDERER_ACCELERATED);
  if (!data->renderer) {
    fprintf(stderr, "Erreur à la création du renderer SDL : %s\n", SDL_GetError());
    detruitVueSDL(vue);
    return NULL;
  }

  // Chargement de la police
  data->police = TTF_OpenFont("police/DejaVuSans.ttf", 30);
  if (!data->police) {
    fprintf(stderr, "Erreur lors du chargement de la police : %s\n", TTF_GetError());
    detruitVueSDL(vue);
    return NULL;
  }

  // Initialisation des origines des terrains
  x = (FEN_LARG - (nbColonnes * DIM_CASE + 3 * MARGE_COL + DIM * DIM_CASE)) / 2;
  y = (FEN_HAUT - (nbLignes * DIM_CASE + 2 * MARGE_LIG)) / 2;
  data->oTerrain = (Couple){x + MARGE_COL, y + MARGE_LIG};
  data->oSuivante =
      (Couple){x + nbColonnes * DIM_CASE + 2 * MARGE_COL, (FEN_HAUT - DIM * DIM_CASE) / 2};

  // Initialisation du reste des variables
  vue->metVueAJour = metVueAJourSDL;
  vue->ecoute = ecouteSDL;
  vue->detruitVue = detruitVueSDL;
  return vue;
}

/**
 * @brief Implémentation de la fonction detruitVueSDL.
 */
void detruitVueSDL(Vue *vue) {
  if (!vue)
    return;
  VueSDL *data = (VueSDL *)vue->data;
  if (!data) {
    free(vue);
    return;
  }
  // Destruction du rendu
  if (data->renderer)
    SDL_DestroyRenderer(data->renderer);
  // Destruction de la fenêtre
  if (data->fenetre)
    SDL_DestroyWindow(data->fenetre);
  // Destruction de la police
  if (!data->police)
    TTF_CloseFont(data->police);
  // Clôture du TTF
  TTF_Quit();
  // Clôture de SDL
  SDL_Quit();
  // Libération des mémoires
  free(data);
  free(vue);
}

/**
 * @brief Implémentation de la fonction getSDLColor.
 */
SDL_Color getSDLColor(Couleur couleur) {
  switch (couleur) {
    case BLANC :
      return (SDL_Color){255, 255, 255, 255};
    case NOIR :
      return (SDL_Color){30, 30, 30, 255};
    case ROUGE :
      return (SDL_Color){255, 0, 0, 255};
    case BLEU :
      return (SDL_Color){0, 255, 0, 255};
    case VERT :
      return (SDL_Color){0, 0, 255, 255};
    case JAUNE :
      return (SDL_Color){255, 255, 0, 255};
    case MAGENTA :
      return (SDL_Color){255, 0, 255, 255};
    case CYAN :
      return (SDL_Color){0, 255, 255, 255};
    default :
      return (SDL_Color){0, 255, 255, 255};
  }
}

/**
 * @brief Implémentation de la fonction dessineRectBordures.
 */
uint8_t dessineRectBordures(VueSDL *data, SDL_Rect *rect, Couleur couleur, float epais) {
  SDL_FRect rect1;
  SDL_Color c = getSDLColor(couleur);
  // Rempli le rectangle de la couleur spécifiée
  if (SDL_SetRenderDrawColor(data->renderer, c.r, c.g, c.b, c.a) < 0) {
    fprintf(stderr, "Erreur lors du dessin d'un rectangle : %s\n", SDL_GetError());
    return 1;
  }
  if (SDL_RenderFillRect(data->renderer, rect) < 0) {
    fprintf(stderr, "Erreur lors du dessin d'un rectangle : %s\n", SDL_GetError());
    return 1;
  }
  // Trace les bordures avec l'épaisseur
  if (SDL_SetRenderDrawColor(data->renderer, 200, 200, 200, 255) < 0) {
    fprintf(stderr, "Erreur lors du dessin d'un rectangle : %s\n", SDL_GetError());
    return 1;
  }
  rect1 = (SDL_FRect){rect->x, rect->y, rect->w, -1 * epais};
  if (SDL_RenderFillRectF(data->renderer, &rect1) < 0) {
    fprintf(stderr, "Erreur lors du dessin d'un rectangle : %s\n", SDL_GetError());
    return 1;
  }
  rect1 = (SDL_FRect){rect->x, rect->y, -1 * epais, rect->h};
  if (SDL_RenderFillRectF(data->renderer, &rect1) < 0) {
    fprintf(stderr, "Erreur lors du dessin d'un rectangle : %s\n", SDL_GetError());
    return 1;
  }
  rect1 = (SDL_FRect){rect->x + rect->w - epais, rect->y, epais, rect->h};
  if (SDL_RenderFillRectF(data->renderer, &rect1) < 0) {
    fprintf(stderr, "Erreur lors du dessin d'un rectangle : %s\n", SDL_GetError());
    return 1;
  }
  rect1 = (SDL_FRect){rect->x, rect->y + rect->h - epais, rect->w, epais};
  if (SDL_RenderFillRectF(data->renderer, &rect1) < 0) {
    fprintf(stderr, "Erreur lors du dessin d'un rectangle : %s\n", SDL_GetError());
    return 1;
  }
  return 0;
}

/**
 * @brief Implémentation de la fonction ecritTexte.
 */
uint8_t ecritTexte(VueSDL *data, SDL_Rect *rect, char *s) {
  SDL_Surface *textSurface;
  // Utilisation d'une surface SDL pour le texte
  textSurface = TTF_RenderText_Blended(data->police, s, getSDLColor(BLANC));
  if (!textSurface) {
    fprintf(stderr, "Erreur lors de la création de la surface de texte : %s\n", TTF_GetError());
    return 1;
  }
  // Créer une texture SDL à partir de la surface de texte
  SDL_Texture *textTexture = SDL_CreateTextureFromSurface(data->renderer, textSurface);
  if (!textTexture) {
    fprintf(stderr, "Erreur lors de la création de la texture de texte : %s\n", SDL_GetError());
    return 1;
  }
  SDL_FreeSurface(textSurface);
  // Afficher la texture de texte
  SDL_RenderCopy(data->renderer, textTexture, NULL, rect);
  return 0;
}

/**
 * @brief Implémentation de la fonction dessineCaseSDL.
 */
uint8_t dessineCaseSDL(Vue *vue, uint16_t x, uint16_t y, Couleur couleur) {
  SDL_Rect rect;
  rect.x = ((VueSDL *)vue->data)->oTerrain.x + x * DIM_CASE;
  rect.y = ((VueSDL *)vue->data)->oTerrain.y + y * DIM_CASE;
  rect.w = DIM_CASE;
  rect.h = DIM_CASE;
  return dessineRectBordures(((VueSDL *)vue->data), &rect, couleur, 0.5);
}

/**
 * @brief Implémentation de la fonction dessineCaseSuivanteSDL.
 */
uint8_t dessineCaseSuivanteSDL(Vue *vue, uint16_t x, uint16_t y, Couleur couleur) {
  SDL_Rect rect;
  rect.x = ((VueSDL *)vue->data)->oSuivante.x + x * DIM_CASE;
  rect.y = ((VueSDL *)vue->data)->oSuivante.y + y * DIM_CASE;
  rect.w = DIM_CASE;
  rect.h = DIM_CASE;
  return dessineRectBordures(((VueSDL *)vue->data), &rect, couleur, 0.5);
}

/**
 * @brief Implémentation de la fonction dessineFormeSDL.
 */
uint8_t dessineFormeSDL(Vue *vue, Couple *coords, Couleur couleur) {
  // On parcours les coordonées
  for (int i = 0; i < NB_CASES_FORME; i++)
    // On vérifie si la forme ne deborde pas sur la base
    if (0 <= coords[i].y - BASE)
      if (dessineCaseSDL(vue, coords[i].x, coords[i].y - BASE, couleur))
        return 1;
  return 0;
}

/**
 * @brief Implémentation de la fonction dessineFormeSuivanteSDL.
 */
uint8_t dessineFormeSuivanteSDL(Vue *vue, Couple *coords, Couleur couleur) {
  // Nettoie le terrain de la forme suivante
  nettoieTerrainSuivanteSDL(vue);
  // On parcours les coordonées et on dessine
  for (int i = 0; i < NB_CASES_FORME; i++)
    if (dessineCaseSuivanteSDL(vue, coords[i].x + 2, coords[i].y + 2, couleur))
      return 1;
  return 0;
}

/**
 * @brief Implémentation de la fonction dessineTerrainSDL.
 */
uint8_t dessineTerrainSDL(Vue *vue, Couleur *terrain) {
  int i, j;
  // On parcours et on dessine
  for (i = 0; i < vue->nbLignes; i++)
    for (j = 0; j < vue->nbColonnes; j++)
      if (dessineCaseSDL(vue, j, i, terrain[i * vue->nbColonnes + j]))
        return 1;
  return 0;
}

/**
 * @brief Implémentation de la fonction nettoieTerrainSuivante.
 */
uint8_t nettoieTerrainSuivanteSDL(Vue *vue) {
  int i, j;
  // On parcours et on nettoie
  for (i = 0; i < DIM; i++)
    for (j = 0; j < DIM; j++)
      if (dessineCaseSuivanteSDL(vue, j, i, NOIR))
        return 1;
  return 0;
}

/**
 * @brief Implémentation de la fonction afficheScoreSDL.
 */
uint8_t afficheScoreSDL(Vue *vue, uint16_t score) {
  SDL_Rect rect;
  VueSDL *data = (VueSDL *)vue->data;
  rect.x = data->oSuivante.x;
  rect.y = data->oSuivante.y + DIM * DIM_CASE + MARGE_LIG;
  rect.w = DIM * DIM_CASE;
  rect.h = DIM_CASE;
  if (dessineRectBordures(data, &rect, NOIR, 0.5))
    return 1;
  char s[25];
  sprintf(s, "S C O R E : %d", score);
  rect.x = rect.x + rect.w / 8;
  rect.y = rect.y + rect.h / 8;
  rect.w = 6 * rect.w / 8;
  rect.h = 6 * rect.h / 8;
  return ecritTexte(data, &rect, s);
}

/**
 * @brief Implémentation de la fonction afficheMessageSDL.
 */
uint8_t afficheMessageSDL(Vue *vue) {
  SDL_Rect rect;
  VueSDL *data = (VueSDL *)vue->data;
  rect.x = data->oSuivante.x;
  rect.y = data->oSuivante.y + (DIM + 1) * DIM_CASE + 2 * MARGE_LIG - 5;
  rect.w = DIM * DIM_CASE;
  rect.h = 5 * (3 * DIM_CASE / 4) + 10;
  if (dessineRectBordures(data, &rect, NOIR, 0.5))
    return 1;
  rect.x += 10;
  rect.y += 5;
  rect.w -= 20;
  rect.h = (3 * DIM_CASE / 4);
  if (ecritTexte(data, &rect, "FLECHE pour diriger"))
    return 1;
  rect.y = rect.y + rect.h;
  if (ecritTexte(data, &rect, "ESPACE pour tourner"))
    return 1;
  rect.y = rect.y + rect.h;
  if (ecritTexte(data, &rect, "ENTREE pour pauser/jouer"))
    return 1;
  rect.y = rect.y + rect.h;
  if (ecritTexte(data, &rect, "R pour recommencer"))
    return 1;
  rect.y = rect.y + rect.h;
  if (ecritTexte(data, &rect, "ECHAP  pour sortir"))
    return 1;
  return 0;
}

/**
 * @brief Implémentation de la fonction ecouteSDL.
 */
Evenement ecouteSDL() {
  SDL_Event event;
  if (SDL_PollEvent(&event))
    switch (event.type) {
      case SDL_QUIT :
        return ECHAP;
      case SDL_KEYDOWN :
        switch (event.key.keysym.sym) {
          case SDLK_UP :
            return FHAUT;
          case SDLK_DOWN :
            return FBAS;
          case SDLK_LEFT :
            return FGAUCHE;
          case SDLK_RIGHT :
            return FDROITE;
          case SDLK_SPACE :
            return ESPACE;
          case SDLK_RETURN :
          case SDLK_KP_ENTER :
            return ENTREE;
          case SDLK_r :
            return TOUCHE_R;
          case SDLK_ESCAPE :
            return ECHAP;
          default :
            return RIEN;
        }
    }
  return RIEN;
}

/**
 * @brief Implémentation de la fonction metVueAjourSDL.
 */
uint8_t metVueAJourSDL(Vue *vue, Modele *modele, int8_t errEtColl, uint16_t pause, uint16_t fini) {
  if (errEtColl == -1)
    return 1;
  Couple coords[NB_CASES_FORME];
  Couleur terrain[vue->nbLignes * vue->nbColonnes];
  SDL_Rect rect;
  VueSDL *data = (VueSDL *)vue->data;

  // Couleur de fond de la fenêtre
  if (SDL_SetRenderDrawColor(data->renderer, 30, 30, 30, 255) < 0) {
    fprintf(stderr, "Erreur lors du dessin d'un rectangle : %s\n", SDL_GetError());
    return 1;
  }
  if (SDL_RenderClear(data->renderer) < 0) {
    fprintf(stderr, "Erreur lors du dessin d'un rectangle : %s\n", SDL_GetError());
    return 1;
  }

  // Dessin de la box du jeu
  rect.w = vue->nbColonnes * DIM_CASE + 3 * MARGE_COL + DIM * DIM_CASE;
  rect.h = vue->nbLignes * DIM_CASE + 2 * MARGE_LIG;
  rect.x = (FEN_LARG - rect.w) / 2;
  rect.y = (FEN_HAUT - rect.h) / 2;
  if (dessineRectBordures(data, &rect, NOIR, 3))
    return 1;

  // On dessine le terrain
  getTerrain(modele, terrain, 0, BASE, vue->nbColonnes, vue->nbLignes);
  if (dessineTerrainSDL(vue, terrain))
    return 1;
  // On dessine la forme
  getCoordFormeCourante(modele, coords);
  if (dessineFormeSDL(vue, coords, getCouleurFormeCourante(modele)))
    return 1;
  // On dessine la forme suivante
  getCoordFormeSuivante(modele, coords);
  if (dessineFormeSuivanteSDL(vue, coords, getCouleurFormeSuivante(modele)))
    return 1;
  // On met à jour le score
  if (afficheScoreSDL(vue, getScore(modele)))
    return 1;

  // On met le message à jour
  if (afficheMessageSDL(vue))
    return 1;
  SDL_RenderPresent(((VueSDL *)vue->data)->renderer);
  return 0;
}