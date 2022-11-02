/********
Fichier: bibliotheque_images.c
Auteurs: Benjamin Chausse  - chab1704
         Guillaume Malgorn - malg1503
Date: 31 Octobre 2022
Description: Fichier de distribution pour GEN145.
********/

#include "bibliotheque_images.h"


void msg(int warnLevel,char txt[], int err){
  char levelName[4][8] = { "Error", "Warning", "Info", "Debug"};
  if (LOGLEVEL >= warnLevel) {
    for (int i=0;i<60;i++){
      printf("-");
    };
    if (warnLevel == 0){
      printf("\n%s %d: ",levelName[warnLevel],err);
    } else {
      printf("\n%s: ",levelName[warnLevel]);
    }
    printf("%s\n",txt);
  }
}

int pgm_lire(char nom_fichier[], int matrice[MAX_HAUTEUR][MAX_LARGEUR],
             int *p_lignes, int *p_colonnes,
             int *p_maxval, struct MetaData *p_metadonnees) {

		/* Local variables */
    char txt[145]; // An error message shouldn't be longer than a tweet...
    char firstChar = {0};
    int filetype = 0;
    int tonedepth = 0;
    char readline[MAX_CHAINE] = {0};
    char metadataAuthor[32] = {0};
    char metadataDate[11] = {0};
    char metadataLocation[32] = {0};

		/* Open the file */
    msg(INFO,"Opening the file...",OK);
    FILE *fp = fopen(nom_fichier, "r");
    if (!fp) {
      msg(ERROR,"Could not open the file. Does the file exist?", ERREUR_FICHIER);
      return ERREUR_FICHIER;
    } else {
      msg(INFO,"Successfully opened file.",OK);
    }


		/* Check and handle metadata */
    msg(INFO,"Looking for metadata information...",OK);
    firstChar = fgetc(fp);
    if (firstChar == '#'){
      fscanf(fp,"%[a-z A-Z];%[0-9 -];%[a-z A-Z,]\n",
                metadataAuthor,
                metadataDate,
                metadataLocation);

      int metaErrs[3];
      metaErrs[0] = (strlen(metadataAuthor)   == 0) ? 1 : 0;
      metaErrs[1] = (strlen(metadataDate)     == 0) ? 1 : 0;
      metaErrs[2] = (strlen(metadataLocation) == 0) ? 1 : 0;

      sprintf(txt, "Metadata error array -> "
                   "{%d:%d:%d}",
                   metaErrs[0],
                   metaErrs[1],
                   metaErrs[2]);
      msg(DEBUG,txt,OK);
      sprintf(txt,"Detected metadata contents\n"
                  "\tAuthor:   %s\n"
                  "\tDate:     %s\n"
                  "\tLocation: %s",
                  metadataAuthor,
                  metadataDate,
                  metadataLocation);
      msg(DEBUG,txt,OK);
      if (metaErrs[0]+metaErrs[1]+metaErrs[2]>0)
        msg(WARNING,"One or more metadata fields"
                    " could not be read properly.",OK);
      else {
        msg(INFO,"Successfully loaded metadata information.",OK);
      }

      strcpy(p_metadonnees->auteur, metadataAuthor);
      strcpy(p_metadonnees->dateCreation, metadataDate);
      strcpy(p_metadonnees->lieuCreation, metadataLocation);

      sprintf(txt,"Contents of metadata struct\n"
                  "\tAuthor:   %s\n"
                  "\tDate:     %s\n"
                  "\tLocation: %s",
                  p_metadonnees->auteur,
                  p_metadonnees->dateCreation,
                  p_metadonnees->lieuCreation);
      msg(DEBUG,txt,OK);
    } else {
      msg(INFO,"No metadata information present.",OK);
    };

		/* Check if filetype ppm or pgm */
    msg(INFO,"Verifying file format...",OK);
    fscanf(fp,"P%d", &filetype);
    sprintf(txt,"Detected filetype is %d",filetype);
    msg(DEBUG,txt,OK);
    if (filetype != 2) {
      msg(ERROR,"Given file does not have a pgm filetype.",ERREUR_FORMAT);
      return ERREUR_FORMAT;
    } else {
      msg(INFO,"File format is pgm (P2) as required.",OK);
    }

		/* Check file dimensions */
    msg(INFO,"Verifying image dimensions...",OK);
    fscanf(fp,"%d %d",p_colonnes,p_lignes);
    sprintf(txt,"Image dimensions\n"
                "\tHeight: %d\n"
                "\tWidth: %d",
                *p_lignes, *p_colonnes);
    msg(DEBUG,txt,OK);
    if (*p_lignes > MAX_HAUTEUR || *p_colonnes > MAX_LARGEUR) {
      msg(ERROR,"Image height and/or exceeds maximum value.",ERREUR_TAILLE);
      return ERREUR_TAILLE;
    } else {
      sprintf(txt,"Found that image resolution is %dx%d.",
                  *p_colonnes, *p_lignes);
      msg(INFO,txt,OK);
    }

		/* Check file tonedepth precision */
    msg(INFO,"Verifying image tone depth...",OK);
    fscanf(fp,"%d",&tonedepth);
    sprintf(txt,"Read a tone depth value of %d.",tonedepth);
    msg(DEBUG,txt,OK);
    if (tonedepth > MAX_VALEUR) {
      // TODO: check if ERREUR_FORMAT is the right type of error
      msg(ERROR,"Tone depth resolution surpasses what the spec allows.",ERREUR_FORMAT);
      return ERREUR_FORMAT;
    } else {
      sprintf(txt,"Tone depth is set to %d.",tonedepth);
      msg(INFO,txt,OK);
    }


		/* Close the file */
    return OK;
}

int pgm_ecrire(char nom_fichier[], int matrice[MAX_HAUTEUR][MAX_LARGEUR],
               int lignes, int colonnes,
               int maxval, struct MetaData metadonnees) {
    return OK;
}
