/********
Fichier: AutoValidation.h
Auteurs: Alexandre Huppe
         Domingo Palao
         Charles-Amtoine Brunet
Date: 8 decembre 2017
Description: Tests automatises pour la problematique de GEN145
              - inclus du feedback pour les etudiants (console)
              - inclus la generation d'un log pour correction automatique.
              - verification de la compatibilité avec gcc
********/

#ifndef Autovalidation
#define Autovalidation

#include "bibliotheque_images.h"

/* 
-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
Liste des operations a coder dans la problematique
-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  1. Pour les images noir et blanc :
    ( 1) Lire l'image                       (pgm_lire)
    ( 2) Sauvegarder l'image                (pgm_ecrire)
    ( 3) Copier l'image                     (pgm_copier)
    ( 4) Trouver la couleur preponderante   (pgm_couleur_preponderante)
    ( 5) Creer l'histogramme de l'image     (pgm_creer_histogramme)
    ( 6) Eclaircir ou noircir l'image       (pgm_eclaircir_noircir)
    ( 7) Creer le negatif de l'image        (pgm_creer_negatif)
    ( 8) Extraction de l'image              (pgm_extraire)
    ( 9) Detection d'images identiques      (pgm_sont_identiques)
    (10) Rotation de 90 degres              (pgm_pivoter90)
										    
  2. Pour les images en couleur :		    
    (11) Lire l'image                       (ppm_lire)
    (12) Sauvegarder l'image                (ppm_ecrire)
    (13) Copier l'image                     (ppm_copier)
    (14) Detection d'images identiques      (ppm_sont_identiques)
    (15) Rotation de 90 degres              (ppm_pivoter90)
-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
*/

void AutoValidation();	/* Valide toutes les fonctions */

/* fonctions de tests d'une fonction a la fois (celles qui sont toutes appelees par AutoValidation(): */
void test_pgm_lire();
void test_pgm_ecrire();
void test_pgm_copier();
void test_pgm_couleur_preponderante();
void test_pgm_creer_histogramme();
void test_pgm_eclaircir_noircir();
void test_pgm_creer_negatif();
void test_pgm_extraire();
void test_pgm_sont_identiques();
void test_pgm_pivoter90();

void test_ppm_lire();
void test_ppm_ecrire();
void test_ppm_copier();
void test_ppm_sont_identiques();
void test_ppm_pivoter90();

#endif
