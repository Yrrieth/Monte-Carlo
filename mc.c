#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define NUM 10000 // Nombre d'essai de l'algorithme de Monte-Carlo

typedef struct moyenne {
	float moyenne;
	float ecartType;
} moy;

void affiche(char grille[]) {
	int i;
	for(i = 0; i < 9; i++) {
	printf("%2c", grille[i]);
	}
	printf("\n");
	}

char* alloueTab(int n) {
	char *tmp;
	int i;
	tmp = malloc(n * sizeof(char));
	return tmp;
}

char* copieTab(char grille[]) {
	char *copie;
	int i;
	copie = alloueTab(9);
	for (i = 0; i < 9; i++) {
	copie[i] = grille[i];
	}
	return copie;
}

/**
 * Verification des conditions de victoire et retourne un resultat
 * 1 = gagnant, 0 = perdant
 */
int victoire (char grille[], char symbole) {
	int i, tmp;
	int res = 0; // Cette variable sera retourné
	
	for(i = 0; i < 9; i++) {
		if (grille[i] == symbole) {
			tmp = i;

			// Verifie vers le bas
			if (i < 3) { 
				while (i < 6) { 
					if (grille[tmp] == symbole) {
						res++;
						if (res == 3)
							return 1;
						tmp += 3;
					} else {
						res = 0;
						break;
					}
				}
			}
			tmp = i;

			// Verifie vers la droite
			if (i%3 == 0) {
				while (i%3 != 2) { 	 
					if (grille[tmp] == symbole) {
						res++;
						if (res == 3)
							return 1;
						tmp += 1;
					} else {
						res = 0;
						break;
					}
				}
			}
			tmp = i;

			// Verifie la diagonale haut/gauche -> bas/droite 
			if (i == 0) {
				while (i%3 != 2 && i < 6) {
					if (grille[tmp] == symbole) {
						res++;
						if (res == 3)
							return 1;
						tmp += 4;
					} else {
						res = 0;
						break;
					}
				}
			}
			tmp = i;
			
			// Verifie la diagonale bas/gauche -> haut/droite
			if (i == 6) {
				while(i%3 != 2 && i > 2) {
					if (grille[tmp] == symbole) {
						res++;
						if (res == 3)
							return 1;
						tmp -= 2;
					} else {
						res = 0;
						break;
					}
				}
			}
		}
	}
	return 0;
}

/**
 * Deroulement du jeu
 */
int playout (char grille[]) {
	int i, k, num, res; // i et k = index, num = valeur aleatoire, res = resultat
	int max; // Max de coups possibles
	
	char pt = '.';
	char x = 'X';
	char o = 'O';

	while (1) {
		max = 0;
		for (k = 0; k < 9; k++) { // Calcul le nombre de case vide
			if (grille[k] == '.')
			max++;
		}
		if (max == 0) {
			break;
		}

		if (max != 0) {
			num = rand() % 9;
			if (max%2 == 1) {	// Joueur X
				for(i = 0; i < 9; i++) {
					if (i == num) {
						if (grille[num] == '.') {
							grille[i] = x;
						} else {
							continue;
						}
					}
				}
			} else if (max%2 == 0) {	// Joueur O
				for(i = 0; i < 9; i++) {
					if (i == num) {
						if (grille[num] == '.') {
							grille[i] = o;
						} else {
							continue;
						}
					}
				}
			}
			affiche(grille);
		}
		
		res = victoire(grille, x);
		if (res == 1) { // Si joueur X gagne
			break;
		} else {
			res = victoire(grille, o);
			if (res == 1) { // Si joueur 0 gagne
				res = 0;
				break;
			}
		}
	}
	return res;
}

/**
 * Algorithme de Monte-Carlo
 */
moy mc (char grille[]) {
	moy res;
	res.moyenne = 0.0;
	res.ecartType = 0.0;
	float sum = 0.0;
	float sum2 = 0.0;
	int tmp;
	int i;

	char *copie;
	copie = copieTab(grille);
	
	for (i = 0; i < NUM; i++) {
		// printf("Nombre d'essai : %d\n", i + 1);
		// Dans cette boucle for, res.moyenne sert de variable temporaire
		res.moyenne = playout(grille);
		sum = res.moyenne + sum;
		sum2 = res.moyenne * res.moyenne + sum2;
		grille = copieTab(copie);
	}
	
	res.moyenne = sum/NUM;
	res.ecartType = sqrt((sum2/NUM) - pow(res.moyenne, 2));
	return res;
}

int main(int argc, char *argv[]) {
	// Modifier la grille pour vérifier les probabilités de victoire
	//char grille[9] = {'X', 'O', '.', 'X', '.', 'X', 'O', '.', 'O'};
	//char grille[9] = {'.', 'X', '.', '.', '.', 'X', 'O', '.', 'O'};
	char grille[9] = {'.', '.', '.', '.', '.', '.', '.', '.', '.'};
	
	moy res;
	srand(time(NULL));
	clock_t begin = clock();
	res = mc(grille);
	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC; 
	printf("Moyenne: %f Ecart-type: %f\n", res.moyenne, res.ecartType);
	printf("Nombre de tics totaux : %f\n", time_spent);
	
	return 0;
}
