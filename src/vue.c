#include <string.h>

#include "vue.h"
#include "vueNcurses.h"
#include "vueSDL.h"

/**
 * @brief Implémentation de la fonction initVue.
 */
Vue *initVue(char *vtype, uint16_t nbLignes, uint16_t nbColonnes) {
  Vue *ret = NULL;
  if (!strcmp(vtype, "ncurses")) {
    ret = initVueNcurses(nbLignes, nbColonnes);
  } else if (!strcmp(vtype, "sdl")) {
    ret = initVueSDL(nbLignes, nbColonnes);
  }
  return ret;
}