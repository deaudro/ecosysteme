#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#include "ecosys.h"



int main(void) {

  srand(time(NULL));
  int i;
  Animal *proies = NULL;
  Animal *predateurs = NULL;
  for(int i = 0; i < 20; i++){
    ajouter_animal(rand()%SIZE_X, rand()%SIZE_Y,10.0, &proies);
  }

  for(int j = 0; j < 20; j++){
    ajouter_animal(rand()%SIZE_X, rand()%SIZE_Y,10.0, &predateurs);
  }

  // Test des fonctions ecrire_ecosys() et lire_ecosys()

	Animal *proies_lire = NULL;
	Animal *predateurs_lire = NULL;
	ecrire_ecosys("liste_ecosys.txt", proies, predateurs);
	lire_ecosys("liste_ecosys.txt", &proies_lire, &predateurs_lire);


	afficher_ecosys(proies_lire, predateurs_lire); printf("\n");
  














  proies = liberer_liste_animaux(proies);
	predateurs = liberer_liste_animaux(predateurs);

}