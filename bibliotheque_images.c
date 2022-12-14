/********
Fichier: bibliotheque_images.c
Auteurs: Benjamin Chausse  - chab1704
         Guillaume Malgorn - malg1503
Date: 31 Octobre 2022
Description: Fichier de distribution pour GEN145.
********/

#include "bibliotheque_images.h"

/****************************************************************************/
/*                            General operations                            */
/****************************************************************************/

void msg(int warnLevel,char txt[], int err){
  char levelName[4][8] = { "Error", "Warning", "Info", "Debug"};
  if (LOGLEVEL!=0){
    if (LOGLEVEL >= warnLevel) {
      for (int i=0;i<60;i++){
        printf("-");
      };
      if (warnLevel == 1){
        printf("\n%s %d: ",levelName[warnLevel-1],err);
      } else {
        printf("\n%s: ",levelName[warnLevel-1]);
      }
      printf("%s\n",txt);
    }
  }
}

/****************************************************************************/
/*                    Operation on grayscale images (pgm)                   */
/****************************************************************************/

int pgm_lire(char nom_fichier[], int matrice[MAX_HAUTEUR][MAX_LARGEUR],
             int *p_lignes, int *p_colonnes,
             int *p_maxval, struct MetaData *p_metadonnees) {

		/* Local variables */
    char txt[145]; // An error message shouldn't be longer than a tweet...
    char firstChar = {0};
    int filetype = 0;
    int tonedepth = 0;
    int pixelcount = 0;
    char readline[MAX_CHAINE] = {0};
    int  metadata = FALSE;
    char metadataAuthor[MAX_CHAINE] = {0};
    char metadataDate[MAX_CHAINE] = {0};
    char metadataLocation[MAX_CHAINE] = {0};

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
      metadata = TRUE;
      fscanf(fp,"%[a-z A-Z];%[0-9 -];%[a-z A-Z,0-9]\n",
                metadataAuthor,
                metadataDate,
                metadataLocation);

      int metaErrs[3]; // Metadata errors for three entries
      metaErrs[0] = (strlen(metadataAuthor)   == 0) ? TRUE : FALSE;
      metaErrs[1] = (strlen(metadataDate)     == 0) ? TRUE : FALSE;
      metaErrs[2] = (strlen(metadataLocation) == 0) ? TRUE : FALSE;

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
    if (metadata) {
      fscanf(fp,"P%d", &filetype);
    } else {
      fscanf(fp, "%d", &filetype);
    }
    sprintf(txt,"Detected filetype is %d",filetype);
    msg(DEBUG,txt,OK);
    // What to do for pgm functions
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
      msg(ERROR,"Image height and/or width exceeds maximum value.",ERREUR_TAILLE);
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

    /* Load the pixels into the matrix */
    msg(INFO,"Reading pixel data from file...",OK);
    for (int h=0;h < *p_lignes;h++){
      for (int w=0; w< *p_colonnes;w++){
        if (!fscanf(fp,"%d",&matrice[h][w])){
          msg(ERROR,"There was insuficient pixel data in the file.",ERREUR_TAILLE);
          return ERREUR_TAILLE;
        }
        pixelcount++;
      }
    }
    // TODO: Error Management...
    msg(INFO,"Successfully loaded the image into the matrix.",OK);
		/* Close the file */
    fclose(fp);
    return OK;
}

int pgm_ecrire(char nom_fichier[], int matrice[MAX_HAUTEUR][MAX_LARGEUR],
               int lignes, int colonnes,
               int maxval, struct MetaData metadonnees) {
    char txt[MAX_CHAINE];
    int metaContents = 0;
    FILE *fp = fopen(nom_fichier, "w");
    if (!fp) {
      msg(ERROR,"Could not open the file for writing."
                "Do you have the nessessary permissions?", ERREUR_FICHIER);
      return ERREUR_FICHIER;
    } else {
      msg(INFO,"Successfully created and cleared the file.",OK);
    }

    metaContents += (strlen(metadonnees.auteur)       == 0 ) ? 1 : 0;
    metaContents += (strlen(metadonnees.dateCreation) == 0 ) ? 1 : 0;
    metaContents += (strlen(metadonnees.lieuCreation) == 0 ) ? 1 : 0;
    
    switch (metaContents){
      case 3:
        fprintf(fp,"#%s;%s;%s\n",metadonnees.auteur,metadonnees.dateCreation,metadonnees.lieuCreation);
      case 0:
        msg(ERROR,"Incomplete metadata information given.",ERREUR_FORMAT);
        return ERREUR_FORMAT;
      default:
        msg(INFO,"No metadata information was given.",OK);
    }

    fprintf(fp,"P2\n");

    if (lignes>MAX_HAUTEUR || colonnes>MAX_LARGEUR){
      msg(ERROR,"Dimensions exceed maximum allowed value",ERREUR_TAILLE);
      return ERREUR_TAILLE;
    } else {
      fprintf(fp,"%d %d\n",colonnes,lignes);
    }

    if (maxval>MAX_VALEUR){
      msg(ERROR,"Tone depth exceeds what is allowed by the format.",ERREUR);
      return ERREUR;
    } else {
      fprintf(fp,"%d\n",maxval);
    }

    for (int w=0;w<MAX_LARGEUR;w++){
      for (int h=0;h<MAX_HAUTEUR;h++){
        fprintf(fp,"%d ",matrice[h][w]);
      }
    }
    fclose(fp);
}

int pgm_copier(int matrice1[MAX_HAUTEUR][MAX_LARGEUR],
               int lignes1, int colonnes1,
               int matrice2[MAX_HAUTEUR][MAX_LARGEUR],
               int *p_lignes2, int *p_colonnes2){

	for(int i = 0; i<lignes1 ; i++)
	{
		for (int j = 0; j<colonnes1 ; j++)
		{
			matrice2[i][j] = matrice1[i][j];
		}
	}
	if (&matrice2[lignes1 - 1][colonnes1 - 1] == NULL)
	{
		msg(ERROR,"La matrice n'a pas la bonne taille.",ERREUR_TAILLE);
		return ERREUR_TAILLE;
	}
	*p_lignes2 = lignes1;
	*p_colonnes2 = colonnes1;
  return OK;
}

int pgm_creer_histogramme(int matrice[MAX_CHAINE][MAX_LARGEUR],
                          int lignes, int colonnes,
                          int histogramme[MAX_VALEUR+1]){
 int a = 0;
	for (int i = 0; i < MAX_VALEUR + 1; i++)
	{
		histogramme[i] =0;
	}
	for (int i = 0; i < lignes; i++)
	{
		for (int j = 0; j < colonnes; j++)
		{
			a = matrice[i][j];
			histogramme[a]++;
		}
	}
	return OK;
}

int pgm_couleur_preponderante(int matrice[MAX_HAUTEUR][MAX_LARGEUR],
    int lignes, int colonnes){

char txt[145];
	int histogram[MAX_VALEUR +1];
	int b = 0;
	for (int i = 0; i < MAX_VALEUR + 1; i++)
	{
		histogram[i] =0;
	}
	for (int i = 0; i < lignes; i++)
	{
		for (int j = 0; j < colonnes; j++)
		{
			b = matrice[i][j];
			histogram[b]++;
		}
	}

	int frequence =0;
	int couleur_preponderante = 0;

	for(int i=0; i<MAX_VALEUR + 1; i++)
	{
		if (histogram[i] > frequence)
		{
			frequence = histogram[i];
			couleur_preponderante = i;
		}
	}
	sprintf(txt,"La couleur preponderante dans cette image est : %d\n", couleur_preponderante);
	msg(INFO,txt,OK);
	return couleur_preponderante;
}

int pgm_eclaircir_noircir(int matrice[MAX_HAUTEUR][MAX_LARGEUR],
    int lignes, int colonnes, int maxval, int valeur){
  // TODO: Error management
  int thresholdVal;
  int mult;
  int pixel;
  // Threshold should be 0 when adding negative values
  thresholdVal = (valeur < 0) ? 0 : maxval;
  // Checking for overflow will be ( -1*pixel > 0 ) when adding negative values
  mult = (valeur < 0) ? -1 : 1;
  for (int h=0;h<lignes;h++){
    for (int w=0;w<colonnes;w++){
      pixel = matrice[h][w] + valeur;
      // Assign the threshold value if the pixel overflows
      matrice[h][w] = (mult*pixel>thresholdVal) ? thresholdVal : pixel;
    }
  }
  return OK;
}

int pgm_creer_negatif(int matrice[MAX_HAUTEUR][MAX_LARGEUR],
    int lignes, int colonnes, int maxval){

  	for (int i = 0; i < lignes; i++) {
		for (int j = 0; j < colonnes; j++) {
			matrice[i][j] = maxval - matrice[i][j];
		}
	}

	if (&matrice[lignes - 1][colonnes - 1] == NULL)
	{
		return ERREUR;
	}
	return OK;
    }

int pgm_extraire(int matrice[MAX_HAUTEUR][MAX_LARGEUR],
                 int lignes1, int colonnes1,
                 int lignes2, int colonnes2,
                 int *n_lignes, int *n_colonnes){
  // check if the dimensions are valid
  if (lignes1 < lignes2 || colonnes1 < colonnes2){
    msg(ERROR,"Dimensions are invalid.",ERREUR_TAILLE);
    return ERREUR_TAILLE;
  }
  // check if matrix dimensions surpasses the maximum allowed
  if (lignes1>MAX_HAUTEUR || colonnes1>MAX_LARGEUR){
    msg(ERROR,"Dimensions exceed maximum allowed value",ERREUR_TAILLE);
    return ERREUR_TAILLE;
  }
  // check if extracted region surpasses matrix dimensions
  if (lignes2+*n_lignes>lignes1 || colonnes2+*n_colonnes>colonnes1){
    msg(ERROR,"Selected area overflows the matrix boundaries",ERREUR_TAILLE);
    return ERREUR_TAILLE;
  }
  // create a temporary matrix to duplicate the current one
  int tempMat[MAX_HAUTEUR][MAX_LARGEUR];
  for (int i=0;i<lignes1;i++){
    for (int j=0;j<colonnes1;j++){
      tempMat[i][j] = matrice[i][j];
    }
  }
  // update matrix dimesions to match extracted region dimensions
  *n_lignes = lignes2;
  *n_colonnes = colonnes2;
  // copy the selected region to the top right of the original matrix
  for (int i=lignes1-1;i<lignes1-1+*n_lignes;i++){
    for (int j=colonnes1-1;j<colonnes1-1+*n_colonnes;j++){
      matrice[i][j] = tempMat[i-lignes2][j-colonnes2];
    }
  }
  return OK;
}

int pgm_sont_identiques(int matrice1[MAX_HAUTEUR][MAX_LARGEUR],
                        int lignes1, int colonnes1,
                        int matrice2[MAX_HAUTEUR][MAX_LARGEUR],
                        int lignes2, int colonnes2){
  // check if the dimensions are the same
  if (lignes1 != lignes2 || colonnes1 != colonnes2){
    msg(ERROR,"The two images have different dimensions.",ERREUR_TAILLE);
    return ERREUR_TAILLE;
  }
  // check if dimensions are within the limits
  if (lignes1 > MAX_HAUTEUR || lignes2 > MAX_HAUTEUR ||
      colonnes1 > MAX_LARGEUR || colonnes2 > MAX_LARGEUR){
    msg(ERROR,"The dimensions of the images exceed the maximum allowed.",ERREUR_TAILLE);
    return ERREUR_TAILLE;
  }
  // Check if the two images are identical
  for (int h=0;h<lignes1;h++){
    for (int w=0;w<colonnes1;w++){
      if (matrice1[h][w] != matrice2[h][w]){
        return DIFFERENTES;
      }
    }
  }
  return IDENTIQUES;
}

int pgm_pivoter90(int matrice[MAX_HAUTEUR][MAX_LARGEUR],
    int *p_lignes, int *p_colonnes, int sens){
char txt[145];
  
  int matWidth  = *p_colonnes;
  int matHeight = *p_lignes;
  *p_colonnes   = matHeight;
  *p_lignes     = matWidth;
  sprintf(txt,"matWidth: %d matHeight: %d",matWidth,matHeight);
  msg(ERROR,txt,OK);
  /* Temporary matrix for rotation (prevents overwriting) */
  int tmpMat[MAX_HAUTEUR][MAX_LARGEUR];
  /* Write data to a temporary matrix */
  if (matWidth > MAX_LARGEUR || matHeight > MAX_LARGEUR){
    msg(ERROR,"Matrix dimensions too large.",ERREUR_FORMAT);
    return ERREUR_FORMAT;
  } 
  for (int h=0;h<matHeight;h++){
    for (int w=0;w<matWidth;w++){
      tmpMat[h][w] = matrice[h][w];
    }
  }
  switch (sens) {
    case SENS_HORAIRE: // rotates the matrix clockwise
      for (int h=0;h<matHeight;h++){
        for (int w=0;w<matWidth;w++){
          matrice[w][matHeight-h-1] = tmpMat[h][w];
        }
      }
      break;
    case SENS_ANTIHORAIRE: // rotates the matrix counterclockwise
      for (int h=0;h<matHeight;h++){
        for (int w=0;w<matWidth;w++){
          matrice[matWidth-w-1][h] = tmpMat[h][w];
        }
      }
      break;
    default:
      msg(ERROR,"Invalid rotation direction instruction.",ERREUR);
      return ERREUR;
  }
  return OK;
}


/****************************************************************************/
/*                     Operations on color images (ppm)                     */
/****************************************************************************/

int ppm_lire(char nom_fichier[], struct RGB matrice[MAX_HAUTEUR][MAX_LARGEUR],
             int *p_lignes,  int *p_colonnes,  int *p_maxval,
             struct MetaData *p_metadonnees){
  char txt[145];
  char firstChar = {0};
  int filetype = 0;
  int tonedepth = 0;
  int pixelcount = 0;
  char readline[MAX_CHAINE] = {0};
  int metadata = FALSE;
  char metadataAuthor[MAX_CHAINE] = {0};
  char metadataDate[MAX_CHAINE] = {0};
  char metadataLocation[MAX_CHAINE] = {0};

  /* Open the file */
  FILE *fp = fopen(nom_fichier, "r");
  if (!fp){
    msg(ERROR,"Could not open file.",ERREUR_FICHIER);
    return ERREUR_FICHIER;
  } else {
    msg(INFO,"File opened successfully.",OK);
  }

	/* Check and handle metadata */
  msg(INFO,"Looking for metadata information...",OK);
  firstChar = fgetc(fp);
  if (firstChar == '#'){
    metadata = TRUE;
    fscanf(fp,"%[a-z A-Z];%[0-9 -];%[a-z A-Z,0-9]\n",
              metadataAuthor,
              metadataDate,
              metadataLocation);

    int metaErrs[3]; // Metadata errors for three entries
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
                "\tAuthor:   %s\n" "\tDate:     %s\n"
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
  if (metadata) {
    fscanf(fp,"P%d", &filetype);
  } else {
    fscanf(fp, "%d", &filetype);
  }
  sprintf(txt,"Detected filetype is %d",filetype);
  msg(DEBUG,txt,OK);
  // What to do for pgm functions
  if (filetype != 3) {
    msg(ERROR,"Given file does not have a pgm filetype.",ERREUR_FORMAT);
    return ERREUR_FORMAT;
  } else {
    msg(INFO,"File format is ppm (P3) as required.",OK);
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
    msg(ERROR,"Image height and/or width exceeds maximum value.",ERREUR_TAILLE);
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
    msg(ERROR,"Tone depth resolution surpasses what the spec allows.",ERREUR_FORMAT);
    return ERREUR_FORMAT;
  } else {
    sprintf(txt,"Tone depth is set to %d.",tonedepth);
    msg(INFO,txt,OK);
  }

  /* Read pixel rbg values and save them in the RGB matrix */
  msg(INFO,"Reading pixel values...",OK);
  for (int i = 0; i < *p_lignes; i++) {
    for (int j = 0; j < *p_colonnes; j++) {
      fscanf(fp,"%d %d %d",
             &matrice[i][j].valeurR,
             &matrice[i][j].valeurG,
             &matrice[i][j].valeurB);
      pixelcount++;
    }
  }

  return OK;
}

int ppm_ecrire(char nom_fichier[],
               struct RGB matrice[MAX_HAUTEUR][MAX_LARGEUR],
               int lignes, int colonnes, int maxval,
               struct MetaData metadonnees){
  char txt[145];

  // check if there is working metadata
  int metadata = 0;
  metadata += (strlen(metadonnees.auteur) > 0) ? TRUE : FALSE;
  metadata += (strlen(metadonnees.dateCreation) > 0) ? TRUE : FALSE;
  metadata += (strlen(metadonnees.lieuCreation) > 0) ? TRUE : FALSE;

  switch (metadata){
    case 0:
      msg(INFO,"No metadata to write.",OK);
      break;
    case 3:
      msg(INFO,"All metadata fields are filled.",OK);
      break;
    default:
      msg(ERROR,"Metadata error.",ERREUR);
      return ERREUR;
  }

  // open file
  FILE *fp = fopen(nom_fichier,"w");
  if (fp == NULL) {
    msg(ERROR,"Could not open file.",ERREUR_FICHIER);
    return ERREUR_FICHIER;
  }

  // write metadata
  if (metadata == 3) {
    msg(INFO,"Writing metadata...",OK);
    fprintf(fp,"#%s;%s;%s\n",
            metadonnees.auteur,
            metadonnees.dateCreation,
            metadonnees.lieuCreation);
  }

  // write header
  msg(INFO,"Writing header...",OK);
  fprintf(fp,"P3\n");

  // write dimensions
  if (colonnes > MAX_LARGEUR || lignes > MAX_HAUTEUR) {
    msg(ERROR,"Image dimensions exceed maximum value.",ERREUR_TAILLE);
    return ERREUR_TAILLE;
  } else {
    sprintf(txt,"Image dimensions\n"
                "\tHeight: %d\n"
                "\tWidth: %d",
                lignes, colonnes);
    msg(DEBUG,txt,OK);
  }
  msg(INFO,"Writing image dimensions...",OK);
  fprintf(fp,"%d %d\n",colonnes,lignes);

  // write tone depth
  if (maxval > MAX_VALEUR) {
    msg(ERROR,"Tone depth resolution surpasses what the spec allows.",ERREUR_FORMAT);
    return ERREUR_FORMAT;
  } else {
    sprintf(txt,"Tone depth is set to %d.",maxval);
    msg(INFO,txt,OK);
  }
  fprintf(fp,"%d\n",maxval);

  // write pixel values
  msg(INFO,"Writing pixel values...",OK);
  for (int i = 0; i < lignes; i++) {
    for (int j = 0; j < colonnes; j++) {
      fprintf(fp,"%d %d %d ",
              matrice[i][j].valeurR,
              matrice[i][j].valeurG,
              matrice[i][j].valeurB);
    }
    fprintf(fp,"\n");
  }

  // close the file
  fclose(fp);

  return OK;
}

int ppm_copier(struct RGB matrice1[MAX_HAUTEUR][MAX_LARGEUR],
               int lignes1, int colonnes1,
               struct RGB matrice2[MAX_HAUTEUR][MAX_LARGEUR],
               int *p_lignes2, int *p_colonnes2){

  for (int i = 0; i < lignes1; i++) {
		for (int j = 0; j < colonnes1; j++) {
			matrice2[i][j].valeurR = matrice1[i][j].valeurR;
			matrice2[i][j].valeurG = matrice1[i][j].valeurG;
			matrice2[i][j].valeurB = matrice1[i][j].valeurB;
		}
	}

	if (&matrice2[lignes1 - 2][colonnes1 - 2].valeurR == NULL || &matrice2[lignes1 - 2][colonnes1 - 2].valeurG == NULL || &matrice2[lignes1 - 2][colonnes1 - 2].valeurB == NULL) {
		return ERREUR;
	}
	*p_lignes2 = lignes1;
	*p_colonnes2 = colonnes1;
  return OK;
}

int ppm_sont_identiques(struct RGB matrice1[MAX_HAUTEUR][MAX_LARGEUR],
                        int lignes1, int colonnes1,
                        struct RGB matrice2[MAX_HAUTEUR][MAX_LARGEUR],
                        int lignes2, int colonnes2){
  int diff = 0;
  // check if dimensions are the same
  if (lignes1 != lignes2 || colonnes1 != colonnes2) {
    msg(ERROR,"Images are not the same size.",ERREUR_TAILLE);
    return ERREUR_TAILLE;
  }
  // check if dimensions are within the maximum
  if (lignes1 > MAX_HAUTEUR || colonnes1 > MAX_LARGEUR) {
    msg(ERROR,"Image dimensions exceed maximum value.",ERREUR_TAILLE);
    return ERREUR_TAILLE;
  } else if (lignes2 > MAX_HAUTEUR || colonnes2 > MAX_LARGEUR) {
    msg(ERROR,"Image dimensions exceed maximum value.",ERREUR_TAILLE);
    return ERREUR_TAILLE;
  }

  // check pixel by pixel if everything is the same
  for (int i=0;i<lignes1;i++){
    for (int j=0;j<colonnes1;j++){
      diff = (matrice1[i][j].valeurR != matrice2[i][j].valeurR) ?  1 : diff;
      diff = (matrice1[i][j].valeurG != matrice2[i][j].valeurG) ?  1 : diff;
      diff = (matrice1[i][j].valeurB != matrice2[i][j].valeurB) ?  1 : diff;
      if (diff != 0) {
        msg(ERROR,"Images are not identical.",DIFFERENTES);
        return DIFFERENTES;
      }
    }
  }
  return IDENTIQUES;
}

int ppm_pivoter90(struct RGB matrice[MAX_HAUTEUR][MAX_LARGEUR],
    int *p_lignes,  int *p_colonnes, int sens){
  struct RGB matriceTemp[MAX_HAUTEUR][MAX_LARGEUR];
  int matWidth = *p_colonnes;
  int matHeight = *p_lignes;
  *p_colonnes = matHeight;
  *p_lignes = matWidth;
  // Check if dimensions surpass maximum values
  if (matHeight > MAX_HAUTEUR || matWidth > MAX_LARGEUR) {
    msg(ERROR,"Image dimensions exceed maximum value.",ERREUR_FORMAT);
    return ERREUR_FORMAT;
  }
  switch (sens){
    case SENS_HORAIRE: // changes the original matrix
      for (int i = 0; i < matHeight; i++) {
        for (int j = 0; j < matWidth; j++) {
          matrice[j][matHeight - i - 1].valeurR = matriceTemp[i][j].valeurR;
          matrice[j][matHeight - i - 1].valeurG = matriceTemp[i][j].valeurG;
          matrice[j][matHeight - i - 1].valeurB = matriceTemp[i][j].valeurB;
        }
      }
      break;
    case SENS_ANTIHORAIRE: // changes the original matrix
      for (int i = 0; i < matHeight; i++) {
        for (int j = 0; j < matWidth; j++) {
          matrice[matWidth - j - 1][i].valeurR = matriceTemp[i][j].valeurR;
          matrice[matWidth - j - 1][i].valeurG = matriceTemp[i][j].valeurG;
          matrice[matWidth - j - 1][i].valeurB = matriceTemp[i][j].valeurB;
        }
      }
      break;
    default:
      msg(ERROR,"Invalid rotation direction.",ERREUR);
      return ERREUR;
  }
  

  return OK;
}