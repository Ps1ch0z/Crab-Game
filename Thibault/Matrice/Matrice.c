#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include "..\\Modules\Tableaux.h"

main()
{
	int mat[NBLIGMAX][NBCOLMAX], nbc, nbl, diag1, diag2, tab[NBCOLMAX * NBLIGMAX], dim;

	Intro2DimClavier(NBLIGMAX, NBCOLMAX, &nbl, &nbc);

	/*
	//Exercice 1: Diagonales
	IntroMatriceEntiersClavier(nbl, nbc, mat);
	AfficheMatriceEntiers(nbl, nbc, mat);
	CalcSomDiagMat(nbl, nbc, mat, &diag1, &diag2);
	*/
	
	//Exercice 2: Elements différents
	IntroMatriceEntiersAleatoire(nbl, nbc, mat, 0, 20);
	AfficheMatriceEntiers(nbl, nbc, mat);
	CalcElemDif(nbl, nbc, mat, &tab, &dim);
	printf("Les \x82lements diff\x82rents sont : \n");
	AfficheVecteurEntiers(dim, tab);
}