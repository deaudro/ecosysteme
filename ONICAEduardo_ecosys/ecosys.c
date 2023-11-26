//Eduardo ONICA 21213171
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "ecosys.h"
#include <string.h>



float p_changement_direction=0.5;
float p_reproduce_proie=0.3;
float p_reproduce_predateur=0.4;
int temps_repousse_herbe=-15;



Animal *creer_animal(int x, int y, float energie) {
  Animal *na = (Animal *)malloc(sizeof(Animal));
  assert(na);
  na->x = x;
  na->y = y;
  na->energie = energie;
  na->dir[0] = rand() % 3 - 1;
  na->dir[1] = rand() % 3 - 1;
  na->suivant = NULL;
  return na;
}



Animal *ajouter_en_tete_animal(Animal *liste, Animal *animal) {
  assert(animal);
  assert(!animal->suivant);
  animal->suivant = liste;
  return animal;
}

void ajouter_animal(int x, int y,  float energie, Animal **liste_animal) {
  assert(x < SIZE_X);
  assert(y < SIZE_Y);
  assert(x >= 0);
  assert(y >= 0);
  *liste_animal = ajouter_en_tete_animal(*liste_animal, creer_animal(x,y,energie));

}




void enlever_animal(Animal **liste, Animal *animal) {
	Animal *listept = *liste;
	Animal *tmp = NULL;
	
	if (listept == animal){
		*liste = (*liste)->suivant;
    free(listept);
	}
	
	else {
		while(listept){
			if(listept->suivant == animal){ /* si on a trouvé l'animal, on fait le raccordage*/
				tmp = listept->suivant;
				listept->suivant = listept->suivant->suivant;
				free(tmp);
			}
			
			else listept = listept->suivant;
		}
	}
}


Animal* liberer_liste_animaux(Animal *liste) {
	Animal *tmp1 = liste;
	Animal *tmp2 = NULL;
	
	while(tmp1){
		tmp2 = tmp1;
		tmp1 = tmp1->suivant;
		free(tmp2);
	}
	
	return NULL;
}

unsigned int compte_animal_rec(Animal *la) {
  if (!la) return 0;
  return 1 + compte_animal_rec(la->suivant);
}

unsigned int compte_animal_it(Animal *la) {
  int cpt=0;
  while (la) {
    ++cpt;
    la=la->suivant;
  }
  return cpt;
}



void afficher_ecosys(Animal *liste_proie, Animal *liste_predateur) {
  unsigned int i, j;
  char ecosys[SIZE_X][SIZE_Y];
  Animal *pa=NULL;

  /* on initialise le tableau */
  for (i = 0; i < SIZE_X; ++i) {
    for (j = 0; j < SIZE_Y; ++j) {
      ecosys[i][j]=' ';
    }
  }

  /* on ajoute les proies */
  pa = liste_proie;
  while (pa) {
    ecosys[pa->x][pa->y] = '*';
    pa=pa->suivant;
  }

  /* on ajoute les predateurs */
  pa = liste_predateur;
  while (pa) {
      if ((ecosys[pa->x][pa->y] == '@') || (ecosys[pa->x][pa->y] == '*')) { /* proies aussi present moi: si on tombe sur case contestée ou avec que proies */
        ecosys[pa->x][pa->y] = '@';
      } else {
        ecosys[pa->x][pa->y] = 'O';
      }
    pa = pa->suivant;
  }

  /* on affiche le tableau */
  printf("+");
  for (j = 0; j < SIZE_Y; ++j) {
    printf("-");
  }  
  printf("+\n");
  for (i = 0; i < SIZE_X; ++i) {
    printf("|");
    for (j = 0; j < SIZE_Y; ++j) {
      putchar(ecosys[i][j]);
    }
    printf("|\n");
  }
  printf("+");
  for (j = 0; j<SIZE_Y; ++j) {
    printf("-");
  }
  printf("+\n");
  int nbproie=compte_animal_it(liste_proie);
  int nbpred=compte_animal_it(liste_predateur);
  
  printf("Nb proies : %5d\tNb predateurs : %5d\n", nbproie, nbpred);

}


void clear_screen() {
  printf("\x1b[2J\x1b[1;1H");  /* code ANSI X3.4 pour effacer l'ecran */
}

void ecrire_ecosys(const char *nom_fichier, Animal *liste_proie, Animal *liste_predateur){
	FILE *f = fopen(nom_fichier, "w");
	if (f == NULL){
		printf("Erreur d'ouverture");
		exit(1);
	}

	fprintf(f, "<proie>\n");
	Animal *tmp = liste_proie;
	while(tmp){
		fprintf(f, "x=%d y=%d dir=[%d,%d] e=%f\n", tmp->x, tmp ->y, tmp->dir[0], tmp->dir[1], tmp->energie);
		tmp = tmp->suivant;
	}
	fprintf(f,"</proie>\n");

	fprintf(f,"<predateur>\n");
	tmp = liste_predateur;
	while(tmp){
		fprintf(f, "x=%d y=%d dir=[%d,%d] e=%f\n", tmp->x, tmp ->y, tmp->dir[0], tmp->dir[1], tmp->energie);
		tmp = tmp->suivant;
	}
	fprintf(f, "</predateur>\n");
	
	fclose(f);

void lire_ecosys(const char *nom_fichier, Animal **liste_predateur, Animal **liste_proie){
	FILE *f = fopen(nom_fichier, "r");
	if (f == NULL){
		printf("Erreur d'ouverture");
		exit(1);
	}

	char buffer[256]; //taille max de ligne 256
	fgets(buffer, 256, f); 
	assert(strncmp(buffer, "<proie>", 7) == 0); /*on assert que le buffer est égal à <proie>, donc qu'on est bien au début de la liste*/

	int x_lu, y_lu, dir0, dir1; float e_lu;
	fgets(buffer, 256, f);
	while(strncmp(buffer, "</proie>", 8) != 0){
		sscanf(buffer, "x=%d y=%d dir=[%d,%d] e=%f\n", &x_lu, &y_lu, &dir0, &dir1, &e_lu);
		Animal *a = creer_animal(x_lu, y_lu, e_lu);
		a->dir[0] = dir0; 
		a->dir[1] = dir1;
		a->suivant = *liste_proie;                /*cette boucle permet de lire le contenu de la liste puis de fermer la balise*/
		*liste_proie = a;
		fgets(buffer, 256, f);
	}

	fgets(buffer, 256, f);                    /*on est arrivé à la fin de notre liste proie, on passe à prédateurs*/
	assert(strncmp(buffer, "<predateur>", 11) == 0);

	fgets(buffer, 256, f);
	while(strncmp(buffer, "</predateur>", 12) != 0){
		sscanf(buffer, "x=%d y=%d dir=[%d,%d] e=%f\n", &x_lu, &y_lu, &dir0, &dir1, &e_lu);
		Animal *a = creer_animal(x_lu, y_lu, e_lu);
		a->dir[0] = dir0; 
		a->dir[1] = dir1;
		a->suivant = *liste_predateur;
		*liste_predateur = a;
		fgets(buffer, 256, f);
	}

	fclose(f);
}


void bouger_animaux(Animal *la) {
  Animal *a = la;
  
	while(a){
		if (rand()/(float)RAND_MAX < p_changement_direction){
			a->dir[0] = rand()%3 - 1;
			a->dir[1] = rand()%3 - 1;
		}
		
		a->x = (a->x + a->dir[0] + SIZE_X) % SIZE_X; 
		a->y = (a->y + a->dir[1] + SIZE_Y) % SIZE_Y;
		a = a->suivant;
	}
}


void reproduce(Animal **la, float p_reproduce) {
  Animal *a = la ? *la : NULL;

  while(a){
    if(rand()/(float)RAND_MAX < p_reproduce){
      ajouter_animal(a->x, a->y, a->energie/2.0, la);
      a->energie = a->energie/2.0;
    }

    a = a->suivant;
  }
}



void rafraichir_proies(Animal **liste_proie, int monde[SIZE_X][SIZE_Y]) {
  bouger_animaux(*liste_proie);
  Animal *temp;
  if(liste_proie){
    temp = *liste_proie;
  }
  else{
    temp = NULL;

  }


  while(temp){ 
    temp->energie--;  //on enlève 1 d'énergie à chaque proie
    if (temp->energie < 0){
      enlever_animal(liste_proie, temp);  //on supprime celles n'en ayant plus
    }

    else if (monde[temp->x][temp->y] > 0){
      temp->energie += monde[temp->x][temp->y]; //la proie peut éventuellement manger l'herbe du monde
      monde[temp->x][temp->y] = temps_repousse_herbe;
    }

    temp = temp->suivant;
  }

  reproduce(liste_proie, p_reproduce_proie); //les animaux se reproduisent éventuellement
}

/* Part 2. Exercice 7, question 1 */
Animal *animal_en_XY(Animal *l, int x, int y) {
  Animal *temp = l;
  while(temp){
    if ((temp->x == x) && (temp->y == y))
      return temp;
    
    temp = temp->suivant;
  }

  return NULL;
} 

void rafraichir_predateurs(Animal **liste_predateur, Animal **liste_proie) {
  bouger_animaux(*liste_predateur);
  Animal *temp = liste_predateur ? *liste_predateur : NULL;

  while(temp){
    temp->energie--;

    if (temp->energie < 0){
      enlever_animal(liste_predateur, temp);
    }
    else {
      Animal *aManger = animal_en_XY(*liste_proie, temp->x, temp->y);
      if (aManger != NULL){
        temp->energie += aManger->energie;
        enlever_animal(liste_proie, aManger);
      }
    }

    temp = temp->suivant;
  }

  reproduce(liste_predateur, p_reproduce_predateur);
}

void rafraichir_monde(int monde[SIZE_X][SIZE_Y]){
  for(int i = 0; i < SIZE_X; i++)
    for(int j = 0; j < SIZE_Y; j++)
      monde[i][j]++;
}

