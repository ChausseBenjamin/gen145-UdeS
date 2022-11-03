/********
Fichier: gestion_images.c
Auteurs: Benjamin Chausse  - chab1704
         Guillaume Malgorn - malg1503
Date: 31 Octobre 2022
Description: Fichier de distribution pour GEN145.
********/

#include <stdio.h>
#include "AutoValidation.h"
#include "bibliotheque_images.h"

int image1[MAX_HAUTEUR][MAX_LARGEUR];
int image2[MAX_HAUTEUR][MAX_LARGEUR];
struct RGB imageRGB1[MAX_HAUTEUR][MAX_LARGEUR];
struct RGB imageRGB2[MAX_HAUTEUR][MAX_LARGEUR];

int main() {

  AutoValidation();

  // void test_pgm_lire();
  // void test_pgm_ecrire();
  // void test_pgm_copier();
  // void test_pgm_couleur_preponderante();
  // void test_pgm_creer_histogramme();
  // void test_pgm_eclaircir_noircir();
  // void test_pgm_creer_negatif();
  // void test_pgm_extraire();
  // void test_pgm_sont_identiques();
  // void test_pgm_pivoter90();

  // void test_ppm_lire();
  // void test_ppm_ecrire();
  // void test_ppm_copier();
  // void test_ppm_sont_identiques();
  // void test_ppm_pivoter90();

    /* int err = 0; */
    /* int lignes1, colonnes1; */
    /* int lignes2, colonnes2; */
    /* int maxval; */
    /* int histogramme[MAX_VALEUR+1]; */
    /* char nom[MAX_CHAINE]; */
    /* struct MetaData metadonnees; */
    /* msg(INFO,"Debut!",0); */
    /* // TODO: Remove this from production code */
    /* strcpy(nom,"img/cat.pgm"); */
    /* // exemple d'appel de fonction */
    /* err = pgm_lire(nom, image1, */
    /*                   &lignes1, &colonnes1, */
    /*                   &maxval, &metadonnees); */
	/* // exemple de traitement d'un code de retour (erreur ou reussite) */
	/* if (err == FALSE) */
		/* msg(INFO,"Aucune erreur.",OK); */
	/* else */
		/* msg(ERROR,"Un problème est survenu",ERREUR_FICHIER); */
	/* // autre exemple d'appel de fonction */
    /* pgm_ecrire(nom, image1, */
    /*            lignes1, colonnes1, */
    /*            maxval, metadonnees); */
    /* msg(INFO,"Fin!",OK); */

    return 0;
}
