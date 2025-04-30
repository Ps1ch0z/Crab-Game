#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>// car on utilise des malocs et des free 
#include "..\\Modules\Lecture\Lecture.h"
#define MAXLIGNE 512

int recup_entier(int* entier, char* ligne);
int** allouer_tableau(int NBPOSTE, int** dimfiles, int** tempsrestant, int** temstraitement);
void libérer_tableau(int** dimfiles, int NBPOSTE, int*** fileattente, int** tempsrestant, int** tempstraitement);
void afficher_donnée_départ(int* tempstraitement, int NBPOSTE, int périodicité, int duréesimu);
void afficher_donnée(int temps, int** dimfiles, int** tempsrestant, int*** filesattente, int NBPOSTE);
int ajouté_produit(int produit_à_bouger, int postefinal, int*** files_attente, int** dimfiles);
int Supprimé_Produit(int Nduposte, int*** filesattente, int** tempstraitement, int** tempsrestant, int** dimfiles);
int recup_entier(int* entier, char* ligne);
int estchiffre(char caractère);
int recupfichierCSV(int* NBPOSTE, int** tempstraitement, int* périodicité, int* duréesimu, int** dimfiles, int** tempsrestant, int*** filesattente);
void enregistrer_donnée(FILE* fichier, int temps, int** dimfiles, int** tempsrestant, int*** filesattente, int NBPOSTE);
//ouvrir le fichier excel dans le main et écrire dedans avec fct et fermer toute à la fin
main()
{
	int** filesattente = NULL;
	int* dimfiles = NULL, * tempsrestant = NULL, * tempstraitement = NULL;
	int NBPOSTE, périodicité, duréesimu, erreur, i, j, k;
	char* cheminacces;
	//*cheminacces=LireChaineDynamique("veuillez entrer votre chemin d'acces de votre fichier CSV :");
	erreur = recupfichierCSV(&NBPOSTE, &tempstraitement, &périodicité, &duréesimu, &dimfiles, &tempsrestant, &filesattente);
	if (erreur == -1)
	{
		printf("impossible d'ouvrir le fichier ! ou erreur d'allocation dans la fct recupfichierCSV \n");
		return;
	}
	afficher_donnée_départ(tempstraitement, NBPOSTE, périodicité, duréesimu);
	FILE* fichierOut = fopen("FichierSimuOut", "w");
	if (fichierOut == NULL)
	{
		printf("Erreur d'ouverture du fichier Out\n");
		return;
	}

	for (i = 0; i < duréesimu; i++)
	{
		if (i % périodicité == 0)
		{
			erreur = ajouté_produit(i, 1, &filesattente, &dimfiles);
			if (erreur == -1)
			{
				printf("erreur dans l'alocation de la chaine tampon ajoute produit");
				return;
			}
		}
		for (j = 0; j < NBPOSTE; j++)
		{

			if (filesattente[j][0] != -1)
			{
				tempsrestant[j] = tempsrestant[j] - 1;
				if (tempsrestant[j] == -1)
				{
					erreur = ajouté_produit(filesattente[j][0], j + 2, &filesattente, &dimfiles);
					if (erreur == -1)
					{
						printf("erreur dans l'alocation de la chaine tampon ajoute produit");
						return;
					}
					erreur = Supprimé_Produit(j, &filesattente, &tempstraitement, &tempsrestant, &dimfiles);
					if (erreur == -1)
					{
						printf("erreur dans l'alocation de la chaine tampon ajoute produit");
						return;
					}

				}

			}

		}
		afficher_donnée(i, &dimfiles, &tempsrestant, &filesattente, NBPOSTE);
		enregistrer_donnée(fichierOut, i, &dimfiles, &tempsrestant, &filesattente, NBPOSTE);

	}
	fclose(fichierOut);
	libérer_tableau(&dimfiles, NBPOSTE, &filesattente, &tempsrestant, &tempstraitement);
}

int** allouer_tableau(int NBPOSTE, int** dimfiles, int** tempsrestant, int** tempstraitement)
{
	int i, j, k;

	*tempstraitement = (int*)malloc(NBPOSTE * sizeof(int));
	if (*tempstraitement == NULL)
	{
		return NULL;
	}

	*dimfiles = (int*)malloc(NBPOSTE * sizeof(int));
	if (*dimfiles == NULL)
	{
		free(*tempstraitement);
		return NULL;
	}

	*tempsrestant = (int*)malloc(NBPOSTE * sizeof(int));
	if (*tempsrestant == NULL)
	{
		free(*dimfiles);
		free(*tempstraitement);
		return NULL;
	}

	int** filesattente = (int**)malloc(NBPOSTE * sizeof(int*));
	if (filesattente == NULL)
	{
		free(*dimfiles);
		free(*tempsrestant);
		free(*tempstraitement);
		return NULL;
	}

	for (i = 0; i < NBPOSTE; i++)
	{
		filesattente[i] = (int*)malloc(1 * sizeof(int));
		if (filesattente[i] == NULL)
		{
			for (j = 0; j < i; j++)
			{
				free((*filesattente)[j]);
			}
			free(filesattente);
			free(*dimfiles);
			free(*tempsrestant);
			free(*tempstraitement);
			return NULL;
		}
	}
	for (k = 0; k < NBPOSTE; k++)
	{
		(*dimfiles)[k] = 0;
		filesattente[k][0] = -1;

	}

	return filesattente;
}

void libérer_tableau(int** dimfiles, int NBPOSTE, int*** fileattente, int** tempsrestant, int** tempstraitement)
{
	int i;
	for (i = 0; i < NBPOSTE; i++)//libération de chaque ligne
	{
		free((*fileattente)[i]);
	}
	free(*fileattente);// libération du tableau de pointeur
	free(*dimfiles);
	free(*tempsrestant);
	//free(*tempstraitement);
}

void afficher_donnée_départ(int* tempstraitement, int NBPOSTE, int périodicité, int duréesimu)
{
	int i;

	printf("\n Nombre de postes : %d \t\t Temps de traitement {", NBPOSTE);
	for (i = 0; i < NBPOSTE; i++)
	{
		printf("%d", tempstraitement[i]);
		if (i < NBPOSTE - 1)
			printf(";");
	}
	printf("} \n Periodicite des nouveaux produits : %d \t\t Duree de la simultaion : %d", périodicité, duréesimu);



}

void afficher_donnée(int temps, int** dimfiles, int** tempsrestant, int*** filesattente, int NBPOSTE)//ici les valeurs ne font qu'entrer donc juste besoin du type de variable (pas de leur adresse)
{
	int i, j;
	printf("\n\nTemps %d : ", temps);
	for (i = 0; i < NBPOSTE; i++)
	{
		printf("\n\t poste %d :", i + 1);
		if ((*filesattente)[i][0] == -1)
			printf("\t en traitement \t\ttps restant \t\t en attente");
		else
		{
			printf("\t en traitement %d \ttps restant %d \t\t en attente", (*filesattente)[i][0], (*tempsrestant)[i] + 1);

			for (j = 1; j <= (*dimfiles)[i]; j++)
			{
				printf(" %d", (*filesattente)[i][j]);
			}

		}
	}
}

int ajouté_produit(int produit_à_bouger, int postefinal, int*** files_attente, int** dimfiles)
{
	int i;

	if ((*files_attente)[postefinal - 1][0] == -1)
	{
		(*files_attente)[postefinal - 1][0] = produit_à_bouger;
	}
	else
	{
		int* tampon = (int*)malloc(((*dimfiles)[postefinal - 1] + 2) * sizeof(int));
		if (tampon == NULL)
			return -1;//erreur d'allocations

		for (i = 0; i < (*dimfiles)[postefinal - 1] + 1; i++)
		{
			tampon[i] = (*files_attente)[postefinal - 1][i];

		}
		free((*files_attente)[postefinal - 1]);
		tampon[i] = produit_à_bouger;
		(*files_attente)[postefinal - 1] = tampon;
		(*dimfiles)[postefinal - 1] = (*dimfiles)[postefinal - 1] + 1;

	}

	return 0;// pas d'erreur
}

int Supprimé_Produit(int Nduposte, int*** filesattente, int** tempstraitement, int** tempsrestant, int** dimfiles)// cette fonction enlève le preimière élément de la chaîne du poste
{
	int i = 0;

	if ((*filesattente)[Nduposte][0] != -1)// tout les cas sauf quand la chaine est vide
	{
		if ((*dimfiles)[Nduposte] > 0)// tout les cas sauf quand il n'y rien dans la file d'attente
		{
			int* tampon = (int*)malloc(((*dimfiles)[Nduposte]) * sizeof(int));
			if (tampon == NULL)
				return -1;//erreur d'allocations

			for (i = 0; i < (*dimfiles)[Nduposte]; i++)
			{
				tampon[i] = (*filesattente)[Nduposte][i + 1];
			}
			free((*filesattente)[Nduposte]);
			(*filesattente)[Nduposte] = tampon;
			(*tempsrestant)[Nduposte] = (*tempstraitement)[Nduposte];//remise du minuteur à sa valeur initiale
			(*dimfiles)[Nduposte] = (*dimfiles)[Nduposte] - 1;
		}

		else // cas ou on a un objet finit et rien en fileattente
		{
			(*filesattente)[Nduposte][0] = -1;

		}

	}
	return 0;//pas d'erreur
}

int recup_entier(int* entier, char* ligne)
{
	int i = 0;

	while (ligne[i] != ';' && estchiffre(ligne[i + 1]) != 1)
	{
		i = i + 1;
	}

	sscanf_s(&ligne[i + 1], "%d", entier);//une fois qu'on connait le nombre de poste alors peut savoir dimension de temps traitement
	return  i + 2;
}

int recupfichierCSV(int* NBPOSTE, int** tempstraitement, int* périodicité, int* duréesimu, int** dimfiles, int** tempsrestant, int*** filesattente)
{
	int i = 0, j = 0, entier, position, newposition = 0;
	char ligne[MAXLIGNE + 1];
	FILE* pfich;

	pfich = fopen("FichierSimuIn.csv","r");
	if (pfich == NULL)
	{
		return -1; // Erreur lors de l'ouverture du fichier
	}

	while (fgets(ligne, MAXLIGNE + 1, pfich) != NULL)
	{
		position = 0;

		if (j == 0)
		{
			position = recup_entier(&entier, &ligne[position]);
			*NBPOSTE = entier;
			*filesattente = allouer_tableau(entier, dimfiles, tempsrestant, tempstraitement);
			if (filesattente == NULL)
			{
				return -1;
			}
		}
		else if (j == 1)
		{
			for (i = 0; i < *NBPOSTE; i++)
			{
				position = recup_entier(&entier, &ligne[newposition]);
				newposition = newposition + position;
				(*tempstraitement)[i] = entier;
				(*tempsrestant)[i] = entier;
			}
		}
		else if (j == 2)
		{
			position = recup_entier(&entier, &ligne[position]);
			*périodicité = entier;
		}
		else if (j == 3)
		{
			position = recup_entier(&entier, &ligne[position]);
			*duréesimu = entier;
		}
		j++;
	}

	fclose(pfich);
	return 0;
}

int estchiffre(char caractère)
{
	if (caractère >= '0' && caractère <= '9')
		return 1;
	else
		return -1;

}

void enregistrer_donnée(FILE* fichier, int temps, int** dimfiles, int** tempsrestant, int*** filesattente, int NBPOSTE)
{
	int i, j;
	fprintf(fichier, "Temps;%d\n", temps);
	for (i = 0; i < NBPOSTE; i++)
	{
		fprintf(fichier, "Poste;%d;", i + 1);
		if ((*filesattente)[i][0] == -1)
		{
			fprintf(fichier, "En traitement;vide;Temps restant;-1;En attente;vide\n");
		}
		else
		{
			fprintf(fichier, "En traitement;%d;Temps restant;%d;En attente;", (*filesattente)[i][0], (*tempsrestant)[i] + 1);
			if ((*dimfiles)[i] == 0)
			{
				fprintf(fichier, "vide\n");
			}
			else
			{
				for (j = 1; j <= (*dimfiles)[i]; j++)
				{
					fprintf(fichier, "%d", (*filesattente)[i][j]);
					if (j != (*dimfiles)[i])
						fprintf(fichier, ";");
				}
				fprintf(fichier, "\n");
			}
		}
	}
	fprintf(fichier, "\n"); // Séparation entre deux instants de temps
}
