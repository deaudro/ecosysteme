#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <strings.h>
#include "ecosys.h"
#include <string.h>



#define NB_PROIES 20
#define NB_PREDATEURS 20
#define T_WAIT 40000





int main(void) {
  /* ENLEVER LES SLASH ETOILE POUR TESTER
  printf("test de bouger_animaux() \n");
  Animal *a1 = creer_animal(10, 0, 10.0); //nous testons également la toricité en meme temps
  assert(a1->x >= 0 && a1->x < SIZE_X && a1->y >= 0 && a1->y < SIZE_Y);

  printf("Début du test: %d, %d, %.2f, [%d,%d]\n", a1->x, a1->y, a1->energie, a1->dir[0], a1->dir[1]);
  while((a1->x == 10) && (a1->y == 0)) //l'animal ne bouge pas tt le temps, on veut s'assurer qu'il bouge au moins une fois
    bouger_animaux(a1);
  printf("fin du test: %d, %d, %.2f, [%d,%d]\n", a1->x, a1->y, a1->energie, a1->dir[0], a1->dir[1]);

  printf("\n");

  // Test pour le reproduce();

  for(int i = 0; i < 2; i++)
    reproduce(&a1, p_reproduce_proie);

  printf("Test de reproduce(): \n");
  Animal *temp = a1;
  while(temp){
    printf("%d, %d, %.2f, [%d,%d]\n", a1->x, a1->y, a1->energie, a1->dir[0], a1->dir[1]);
    temp = temp->suivant;
  }

  printf("fin du test \n");
  a1 = liberer_liste_animaux(a1);
  */
   // On crée un tableau[sizex][sizey] avec aucune herbe
	int monde[SIZE_X][SIZE_Y] = {0};
  int energie_proie = 10;
  int energie_predateur = 10;

  // On crée 20 proies et 20 prédateurs.

  Animal *liste_proie = creer_animal(rand() % SIZE_X, rand() % SIZE_Y, energie_proie);
  Animal *liste_predateur = creer_animal(rand() % SIZE_X, rand() % SIZE_Y, energie_predateur);
	
	for(int i = 0; i < 19; i++){
		ajouter_animal(rand() % SIZE_X, rand() % SIZE_Y, energie_proie, &liste_proie);

    ajouter_animal(rand() % SIZE_X, rand() % SIZE_Y, energie_predateur, &liste_predateur);
    ajouter_animal(rand() % SIZE_X, rand() % SIZE_Y, energie_predateur, &liste_predateur);
    ajouter_animal(rand() % SIZE_X, rand() % SIZE_Y, energie_predateur, &liste_predateur);
    ajouter_animal(rand() % SIZE_X, rand() % SIZE_Y, energie_predateur, &liste_predateur);

	}


  
   //Lancement de la simulation "on ne s'arrete pas tant qu'il ne reste plus aucun individu des 2 groupes".

  int cpt = 0;
  while((liste_proie || liste_predateur) && cpt<1000){ //on modifie le || en && si on veut s'arreter si l'un des deux groupes disparait
    afficher_ecosys(liste_proie, liste_predateur);
    usleep(99999); clear_screen();
    rafraichir_proies(&liste_proie, monde);
    rafraichir_predateurs(&liste_predateur, &liste_proie);
    rafraichir_monde(monde);
    cpt++;
  }

  liste_proie = liberer_liste_animaux(liste_proie);
  liste_predateur = liberer_liste_animaux(liste_predateur);

  printf("La simulation 1 s'est terminée au bout du %dème tour \n", cpt);
  return 0;
}

  

